#include <iostream>
#include <fstream>
#include <string>
#include <array>

template<typename... Args>
[[nodiscard]] char CommonItem(Args&&... items) {
    uint8_t common_items[128]{0};
    return (... + [&]() -> char {
        uint8_t unique_items[128]{0};
        auto size = items.length();
        for (int i=0; i<size; i++) {
            char item = items[i];
            if (!unique_items[item]) {
                unique_items[item]++;
                if (++common_items[item] == sizeof...(items)) {
                    return item;
                }
            }
        }
        return 0;
    }());
}

[[nodiscard]] char FindCommonItem(std::string& backpack) {
    auto size = backpack.length();
    uint8_t items[128]{0}; // Same amount of memory for array of booleans
    for (int i=0; i<size/2; i++) {
        // Assign items
        auto item1 = backpack[i];
        auto item2 = backpack[size/2+i];
        // First element
        if (items[item1] != 1) {
            if (items[item1] == 2) {
                return item1;
            } else {
                items[item1] = 1;
            }
        }
        // Second element
        if (items[item2] != 2) {
            if (items[item2] == 1) {
                return item2;
            } else {
                items[item2] = 2;
            }
        }
    }
    return '?';
}

[[nodiscard]] int Priority(char& item) {
    if (item < 91) {
        return item - 38;
    }
    return item - 96;
}

int main (int argc, char *argv[]) {
    // Start file stream
    std::ifstream input_file("data.txt");
    if (!input_file) {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }  

    /* Q1 */
    std::string items;
    int priority_sum{0};
    while (input_file >> items) {
        auto common_item= CommonItem(items.substr(0, items.length()/2), items.substr(items.length()/2));
        priority_sum += Priority(common_item);
    }
    std::cout << "Sum of priorities (Q1): " << priority_sum << std::endl;

    // Reset cursor to beginning of input file
    input_file.clear();
    input_file.seekg(0);

    /* Q2 */
    std::string backpack1, backpack2, backpack3;
    priority_sum = 0;
    while (input_file >> backpack1 && input_file >> backpack2 && input_file >> backpack3) {
        auto common_item = CommonItem(backpack1, backpack2, backpack3);
        priority_sum += Priority(common_item);
    }
    std::cout << "Sum of priorities (Q2): " << priority_sum << std::endl;
    
    return 0;
}