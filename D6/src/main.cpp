#include <deque>
#include <fstream>
#include <iostream>
#include <string>

int NextWindowIndex(std::string &data, int window_index, uint8_t window_size) {
    uint8_t indices[128]{0};
    for (uint8_t i = 0; i < window_size; i++) {
        char character = data[window_index + i];
        if (!indices[character]) {
            indices[character] = i + 1;
        } else {
            return window_index + indices[character];
        }
    }
    return -1;
}

int FindMarker(std::string &datastream, uint8_t window_size = 4) {
    int window_index{0};
    while (window_index + window_size <= datastream.length()) {
        int new_window_index =
            NextWindowIndex(datastream, window_index, window_size);
        if (new_window_index == -1) {
            return window_index + window_size;
        }
        window_index = new_window_index;
    }
    return -1;
}

int main(int argc, char *argv[]) {
    // Start file stream
    std::ifstream input_file("data.txt");
    if (!input_file) {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }

    std::string input;
    if (input_file >> input) {
        std::cout << "Start of packet index: " << FindMarker(input, 4)
                  << std::endl;
        std::cout << "Start of message index: " << FindMarker(input, 14)
                  << std::endl;
    }

    return 0;
}