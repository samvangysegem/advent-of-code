#include <iostream>
#include <fstream>
#include <string_view>
#include <array>

// Part 1
// constexpr const std::array<std::array<uint8_t, 3>, 3> SCORE_TABLE{{{{4, 8, 3}},{{1, 5, 9}},{{7, 2, 6}}}};

// Part 2
constexpr const std::array<std::array<uint8_t, 3>, 3> SCORE_TABLE{{{{3, 4, 8}},{{1, 5, 9}},{{2, 6, 7}}}};

[[nodiscard]] uint8_t CalculateScore(uint8_t opponent_move, uint8_t player_move) {
    return SCORE_TABLE[opponent_move - 65][player_move - 88];
}

int main (int argc, char *argv[]) {
    // Start file stream
    std::ifstream input_file("data.txt");
    if (!input_file) {
        std::cerr << "Unable to open file" << std::endl;
        return 2;
    }

    size_t total_score{0};
    uint8_t opponent_move, player_move;
    while (input_file >> opponent_move && input_file >> player_move) {
        total_score += CalculateScore(opponent_move, player_move);
    }

    std::cout << "Total score: " << total_score << std::endl;
    return 0;
}