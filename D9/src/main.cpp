// Run from development folder
// cmake --build ../build --target clean
// cmake --build ../build -j4 -v
// chmod ug+x .git/hooks/* for hooks

#include <fstream>
#include <iostream>
#include <string>

#include <map>
#include <array>
#include <unordered_set>
#include <utility>

//#define PART1
#define PART2

/**
 * Rope model
 * Way of storing positions the tail visited
 * - 2d matrix -> rescaling is required and can become huge -> NOPE
 * - 2d sparse matrix -> linked lists -> bit overkill for storing coordinates
 * - set of coordinate representations (assume x,y < 1000) -> unordered set
 * uint16_t -> ~65000 -> sartt
 */

using Movement = std::pair<int16_t, int16_t>;

Movement naiveNormalise(const Movement &move) {
    return {(move.first != 0) ? move.first / std::abs(move.first) : 0,
            (move.second != 0) ? move.second / std::abs(move.second) : 0};
}

struct Position {
    Position() = default;
    Position(int16_t x, int16_t y) : mX(x), mY(y) {}

    [[nodiscard]] uint16_t distanceTo(const Position &position) noexcept {
        return std::max(std::abs(position.mX - mX), std::abs(position.mY - mY));
    }

    Position &operator+=(const Movement &move) {
        this->mX += move.first;
        this->mY += move.second;
        return *this;
    }

    friend Position operator+(const Position &position, const Movement &move) {
        return {static_cast<int16_t>(position.mX + move.first),
                static_cast<int16_t>(position.mY + move.second)};
    }


    friend Position operator-(const Position &position, const Movement &move) {
        return {static_cast<int16_t>(position.mX - move.first),
                static_cast<int16_t>(position.mY - move.second)};
    }

    int16_t mX{0};
    int16_t mY{0};
};

class Rope {
  public:
    Rope() = default;

    /**
     * @brief Updates the position of the rope head (H) and tail (T) based on
     * the provided move, which should have a magnitude of 1 at most
     * @return Updated position of rope tail (T)
     */
    [[nodiscard]] Position updatePosition(const Movement& move) noexcept {
        mHead += move;
        if (mTail.distanceTo(mHead) > 1) {
            mTail = mHead - move;
        }
        return mTail;
    }

    /**
     * @brief Updates the position of the rope head (H) to the given position
     * @return Updated position of rope tail (T)
     */
     [[nodiscard]] Position updateHeadTo(const Position& position) noexcept {
        if (mHead.distanceTo(position) > 1) {
            std::cout << "Invalid new head position!" << std::endl;
            return mTail;
        }

        mHead = position;

        if (mTail.distanceTo(mHead) <= 1) {
            return mTail;
        }

        /**
         * . . . .   . . . .   . . . .
         * . x H .   . . H .   . H x .
         * T . . .   . . x .   . . . T
         * . . . .   . T . .   . . . .
         *
         * 1. Check the four orthogonal positions around H
         * 2. Choose the one for which the distance to T is 1
         */
         // ToDo: The approach below is really not optimal but works for now...
        static constexpr const std::array<Movement, 4> orthogonalNeighbourMoves{{{-1, 0}, {0, -1}, {1, 0}, {0, 1}}};

        for (const auto& moveToNeighbour: orthogonalNeighbourMoves) {
            auto newTailPosition = mHead + moveToNeighbour;
            if (mTail.distanceTo(newTailPosition) <= 1) {
                mTail = newTailPosition;
                return mTail;
            }
        }

        static constexpr const std::array<Movement, 4> diagonalNeighbourMoves{{{-1, -1}, {-1, 1}, {1, 1}, {1, -1}}};

        for (const auto& moveToNeighbour: diagonalNeighbourMoves) {
            auto newTailPosition = mHead + moveToNeighbour;
            if (mTail.distanceTo(newTailPosition) <= 1) {
                mTail = newTailPosition;
                return mTail;
            }
        }

        std::cout << "Failed to updated Tail position!" << std::endl;
        std::cout << "H: {" << mHead.mX << "," << mHead.mY << "}, T: {" << mTail.mX << "," << mTail.mY << "}\n";

        return mTail;

        /**
         * . . . . .
         * T . . . .
         * . H . . .
         * . . H . .
         * . . . . .
         */
     }

  private:
    Position mHead{0, 0};
    Position mTail{0, 0};
};

int main(int argc, char *argv[]) {
    // Start file stream
    std::ifstream input_file("data.txt");
    if (!input_file) {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }

    std::string line;
    std::unordered_set<int32_t> tailPositions{};
    std::map<char, Movement> commandMap{
        {'D', {0, -1}}, {'R', {1, 0}}, {'U', {0, 1}}, {'L', {-1, 0}}};

    // Part 1
#ifdef PART1
    Rope rope{};

    std::cout << "Reading rope movements..." << std::endl;
    while (std::getline(input_file, line) && line.length() > 0) {
        auto command = line[0];
        auto distance = std::stoi(line.substr(2));
        auto move = commandMap[command];

        for (int step=0; step<distance; step++) {
            auto newTailPosition = rope.updatePosition(move);
            auto poorManHash = newTailPosition.mX * 100000 + newTailPosition.mY;
            if (!tailPositions.contains(poorManHash)) {
                tailPositions.insert(poorManHash);
            }
        }
    }
    std::cout << "Number of visited positions: " << tailPositions.size() << std::endl;
#endif

    // Part 2
#ifdef PART2
    Rope headRope{};
    std::array<Rope, 8> tailRopes{};

    std::cout << "Reading rope movements..." << std::endl;
    while (std::getline(input_file, line) && line.length() > 0) {
        auto command = line[0];
        auto distance = std::stoi(line.substr(2));
        auto move = commandMap[command];

        for (int step=0; step < distance; step++) {
            Position lastTailPosition = headRope.updatePosition(move);

            for (auto& tailRope: tailRopes) {
                auto updatedTailPosition = tailRope.updateHeadTo(lastTailPosition);
                lastTailPosition = updatedTailPosition;
            }

            auto poorManHash = lastTailPosition.mX * 100000 + lastTailPosition.mY;
            if (!tailPositions.contains(poorManHash)) {
                tailPositions.insert(poorManHash);
            }
        }
    }
    std::cout << "Number of visited positions: " << tailPositions.size() << std::endl;
#endif
    return 0;
}