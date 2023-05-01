// Run from development folder
// cmake --build ../build --target clean
// cmake --build ../build -j4 -v
// chmod ug+x .git/hooks/* for hooks

/**
 * Notes
 * - Tree is invisible if another tree
 *
 */

#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <algorithm>
#include <numeric>

struct Tree {
    Tree() = default;
    Tree(uint8_t &length) : length(length) {}

    uint8_t length{0};
    bool visible{false};
};

template <uint8_t Rows> class TreeSquare {
  public:
    TreeSquare() = default;

    void BuildFromFile(std::ifstream &file) {
        std::cout << "Building tree square..." << std::endl;

        std::string line;
        for (uint8_t i = 0; i < Rows; i++) {
            file >> line;
            for (uint8_t j = 0; j < Rows; j++) {
                sizes[i][j].length =
                    static_cast<uint8_t>(std::stoi(line.substr(j, 1)));
            }
        }

        std::cout << "Verify last row!" << std::endl;
        for (uint8_t j = 0; j < Rows; j++) {
            std::cout << static_cast<uint16_t>(sizes[Rows - 1][j].length);
        }
        std::cout << std::endl;
    }

    [[nodiscard]] uint16_t CountVisibleTrees() {
        uint16_t count{0};
        // All sides are visible
        count += 4 * Rows - 4;

        // Definitions
        uint8_t highest;
        auto update_visibility = [&](Tree &tree) {
            if (tree.length <= highest) {
                return;
            }
            highest = tree.length;

            if (!tree.visible) {
                tree.visible = true;
                count++;
            }
        };

        for (int i = 1; i < Rows - 1; i++) {
            // Left visibility
            highest = sizes[i].front().length;
            std::for_each(std::begin(sizes[i]) + 1, std::end(sizes[i]) - 1,
                          update_visibility);

            // Right visibility
            highest = sizes[i].back().length;
            std::for_each(std::rbegin(sizes[i]) + 1, std::rend(sizes[i]) - 1,
                          update_visibility);
        }

        for (int j = 1; j < Rows - 1; j++) {
            // Top visibility
            highest = sizes.front()[j].length;
            for (int i = 1; i < Rows - 1; i++) {
                update_visibility(sizes[i][j]);
            }

            // Bottom visibility
            highest = sizes.back()[j].length;
            for (int i = Rows - 2; i >= 0; i--) {
                update_visibility(sizes[i][j]);
            }
        }
        return count;
    }

    [[nodiscard]] size_t GetHighestScenicScore() {
        std::array<std::array<uint32_t, Rows>, Rows> scenic_scores{1};
        for (int i = 1; i < Rows - 1; i++) {
        }
    }

  private:
    std::array<std::array<Tree, Rows>, Rows> sizes;

    [[nodiscard]] size_t GetScenicScore(uint8_t &row, uint8_t &column) {}
};

int main(int argc, char *argv[]) {
    // Start file stream
    std::ifstream input_file("data.txt");
    if (!input_file) {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }

    // Part 1
    TreeSquare<99> trees;
    trees.BuildFromFile(input_file);
    auto visible_trees = trees.CountVisibleTrees();
    std::cout << "Visible trees: " << visible_trees << std::endl;

    // Part 2

    return 0;
}