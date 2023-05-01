// Run from development folder
// cmake --build ../build --target clean
// cmake --build ../build -j4 -v
// chmod ug+x .git/hooks/* for hooks

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

struct Directory {
    Directory(std::string_view name, uint32_t size) : name(name), size(size) {}
    std::string name;
    uint32_t size;
};

struct Directories {
    std::vector<Directory> explored;
    std::vector<Directory> discovering;
};

void ProcessCommand(std::string &line, Directories &dirs) {
    // ls is noise
    if (line.substr(2, line.find(' ', 2)) == "ls") {
        return;
    }
    // cd ..
    if (line.substr(5) == "..") {
        dirs.explored.push_back(dirs.discovering.back());
        dirs.discovering.pop_back();
        return;
    }
    // cd dir
    auto name = line.substr(5);
    dirs.discovering.push_back({name, 0});
}

void ProcessFile(std::string &line, Directories &dirs) {
    // Ignore dirs
    if (line.substr(0, 3) == "dir") {
        return;
    }
    // Process size
    for (auto &dir : dirs.discovering) {
        dir.size += std::stoi(line.substr(0, line.find(' ')));
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
    directories.explored.insert(std::end(directories.explored),
                                std::begin(directories.discovering),
                                std::end(directories.discovering));
    directories.discovering.clear();

    // Part 1
    auto sum = std::accumulate(std::begin(directories.explored),
                               std::end(directories.explored), 0,
                               [](size_t sum, auto &dir) {
                                   if (dir.size < 100000) {
                                       return sum + dir.size;
                                   }
                                   return sum;
                               });

    std::cout << "Sum of directory sizes lower than 100'000: " << sum
              << std::endl;

    // Part 2
    std::sort(std::begin(directories.explored), std::end(directories.explored),
              [](auto &a, auto &b) { return a.size < b.size; });
    uint32_t free_space = 70'000'000 - directories.explored.back().size;
    std::cout << "Free space: " << free_space << std::endl;
    uint32_t required_space = 30'000'000 - free_space;
    std::cout << "Required space: " << required_space << std::endl;
    auto target_dir = std::find_if(
        std::begin(directories.explored), std::end(directories.explored),
        [&](auto &dir) -> bool { return dir.size >= required_space; });
    std::cout << "Directory size to be deleted: " << (*target_dir).size
              << std::endl;

    return 0;
}