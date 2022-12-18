// Run from development folder
// cmake --build ../build --target clean
// cmake --build ../build -j4 -v
// chmod ug+x .git/hooks/* for hooks

#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

struct Directories {
    std::vector<uint32_t> explored;
    std::vector<uint32_t> discovering;
};

void ProcessCommand(std::string &line, Directories &dirs) {
    // ls is noise
    if (line.find("ls", 2) != std::string::npos) {
        return;
    }
    // cd ..
    if (line.find("..", 5) != std::string::npos) {
        dirs.explored.push_back(dirs.discovering.back());
        dirs.discovering.pop_back();
        return;
    }
    // cd dir
    dirs.discovering.push_back(0);
}

void ProcessFile(std::string &line, Directories &dirs) {
    // Ignore dirs
    if (line.find("dir") != std::string::npos) {
        return;
    }
    // Process size
    for (auto &size : dirs.discovering) {
        size += std::stoi(line.substr(0, line.find(' ')));
    }
}

int main(int argc, char *argv[]) {
    // Start file stream
    std::ifstream input_file("data.txt");
    if (!input_file) {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }

    Directories directories;
    std::string line;
    while (std::getline(input_file, line) && line.length() > 0) {
        if (line.find('$') != std::string::npos) {
            ProcessCommand(line, directories);
        } else {
            ProcessFile(line, directories);
        }
    }

    auto sum = std::accumulate(std::begin(directories.explored),
                               std::end(directories.explored), 0,
                               [](size_t sum, auto &size) {
                                   if (size < 100000) {
                                       return sum + size;
                                   }
                                   return sum;
                               });

    std::cout << sum << std::endl;

    return 0;
}