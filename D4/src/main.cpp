#include <iostream>
#include <fstream>
#include <string>
#include <array>

class Ranges {
public:
    Ranges() = default;
    Ranges(std::string& range1, std::string& range2) {
        range1_lb = std::stoi(range1.substr(0, range1.find("-")));
        range1_ub = std::stoi(range1.substr(range1.find("-") + 1));
        range2_lb = std::stoi(range2.substr(0, range2.find("-")));
        range2_ub = std::stoi(range2.substr(range2.find("-") + 1));
    }

    [[nodiscard]] bool AreNested() {
        return (range2_lb >= range1_lb && range2_ub <= range1_ub) || (range2_lb <= range1_lb && range2_ub >= range1_ub);
    }

    [[nodiscard]] bool AreOverlapping() {
        return AreNested() || 
               (range1_lb <= range2_lb && range1_ub >= range2_lb) || 
               (range2_lb <= range1_lb && range2_ub >= range1_lb);
    }

private:
    int range1_lb{0};
    int range1_ub{0};
    int range2_lb{0};
    int range2_ub{0};
};

int main (int argc, char *argv[]) {
    // Start file stream
    std::ifstream input_file("data.txt");
    if (!input_file) {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }  

    /* Q1 */
    std::string line;
    int nested_pairs{0}, overlapping_pairs{0};
    while (input_file >> line) {
        // Extract ranges
        auto range1 = line.substr(0, line.find(","));
        auto range2 = line.substr(line.find(",") + 1);
        // Run checks
        Ranges ranges{range1, range2};
        nested_pairs += ranges.AreNested();
        overlapping_pairs += ranges.AreOverlapping();
    }
    std::cout << "Nested pairs: " << nested_pairs << std::endl;
    std::cout << "Overlapping pairs: " << overlapping_pairs << std::endl;
    
    return 0;
}