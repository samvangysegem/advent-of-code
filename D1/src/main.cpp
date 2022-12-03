#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <algorithm>
#include <numeric>

int main (int argc, char *argv[]) {
    // Start file stream
    std::ifstream input_file("aoc_day1.txt");
    if (!input_file) {
        std::cerr << "Unable to open file" << std::endl;
        return 2;
    }

    // Array for top calories
    std::array<int, 4> top_calories{0};

    // Extract calories from input data
    int maximum_calories = 0, sum_calories = 0;
    std::string data;
    while (getline(input_file, data)) {
        if (!data.empty()) {
            sum_calories += std::stoi(data);
        } else {
            top_calories[3] = sum_calories;
            sum_calories = 0;
            std::sort(std::begin(top_calories), std::end(top_calories), std::greater<int>());
        }
    }

    std::cout << "Maximum calories: " << top_calories[0] << std::endl;
    std::cout << "2nd calories: " << top_calories[1] << std::endl;
    std::cout << "3rd calories: " << top_calories[2] << std::endl;
    std::cout << "Sum of top 3: " << std::accumulate(std::begin(top_calories), std::end(top_calories) - 1, 0) << std::endl;
    return 0;
}