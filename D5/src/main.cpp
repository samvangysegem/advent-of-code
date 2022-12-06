#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <iterator> // Implementation with std::begin() and std::end() required

/**
 * One "problem" with this code is the use of a class for ship which could reduce performance for rearranging the crates if the input data 
 * file is truly large.
 * This could be improved by placing the data in a struct called Ship, keeping the constructor, and using free functions to edit this data
 * which would get rid of the additional "this" pointer associated with member functions.
*/

struct CrateMover9000 {
    static void Move(std::vector<char>& source, std::vector<char>& destination, int amount) {
        for (int i=0; i<amount; i++) {
            destination.push_back(source.back());
            source.pop_back();
        }
    }
};

struct CrateMover9001 {
    static void Move(std::vector<char>& source, std::vector<char>& destination, int amount) {
        destination.insert(destination.end(), source.end() - amount, source.end());
        for (int i=0; i<amount; i++) {
            source.pop_back();
        }
    }
};

template<class Crane> // Can be moved to the RearrangeCrates() method since only this method depends on this template
class Ship {
public:
    Ship() = default;
    Ship(std::ifstream& file) {
        std::string line;
        while(std::getline(file, line) && line.length() > 0) {
            for (int i=0; i<STACKS; i++) {
                char crate = line[4*i+1];
                if (crate != ' ' && crate > 64) {
                    mStacks[i].insert(mStacks[i].begin(), crate);
                }
            }
        }
    }

    template<class U>
    friend std::ostream& operator<<(std::ostream& os, const Ship<U>& obj);

    void RearrangeCrates(std::ifstream& plan) {
        std::string noise;
        int amount, source, destination;
        while(plan >> noise >> amount >> noise >> source >> noise >> destination) {
            Crane::Move(mStacks[source - 1], mStacks[destination - 1], amount);
        }
    }

    void PrintTopOfStacks() {
        for (int i=0; i<STACKS; i++) {
            std::cout << mStacks[i].back();
        }
        std::cout << std::endl;
    }

private:
    static constexpr const uint8_t STACKS = 9;

    std::array<std::vector<char>, STACKS> mStacks;
};

template<class Crane>
std::ostream& operator<<(std::ostream& os, const Ship<Crane>& ship) {
    for (int i=0; i<ship.STACKS; i++) {
        os << i + 1 << ": ";
        for (const char& crate: ship.mStacks[i]) {
            os << crate << " ";
        }
        os << std::endl;
    }
    return os;
}

int main (int argc, char *argv[]) {
    // Start file stream
    std::ifstream input_file("data.txt");
    if (!input_file) {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }

    Ship<CrateMover9000> old_ship{input_file};
    old_ship.RearrangeCrates(input_file);
    std::cout << old_ship;
    old_ship.PrintTopOfStacks();

    input_file.clear();
    input_file.seekg(0);

    Ship<CrateMover9001> new_ship{input_file};
    new_ship.RearrangeCrates(input_file);
    std::cout << new_ship;
    new_ship.PrintTopOfStacks();

    return 0;
}