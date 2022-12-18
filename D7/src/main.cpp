// Run from development folder
// cmake --build ../build --target clean
// cmake --build ../build -j4 -v

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

// Data structure
struct Node {
    Node(std::string name) : name(name) {}
    Node(std::string name, int size) : name(name), size(size) {}

    int size{0};
    std::string name;
    Node *parent = nullptr;
    std::vector<Node> children;
};

[[nodiscard]] Node &GetChildWithName(Node &node, std::string &name) {
    auto child =
        std::find_if(std::begin(node.children), std::end(node.children),
                     [&](auto &child) { return child.name == name; });
    return *child;
}

[[nodiscard]] size_t GetTotalSize(Node &node) {
    return node.size + std::accumulate(std::begin(node.children),
                                       std::end(node.children), 0,
                                       [](size_t sum, Node &node) {
                                           return sum + GetTotalSize(node);
                                       });
}

// Process input

[[nodiscard]] bool IsCommand(std::string &input) {
    return input.substr(0, input.find(' ')) == "$";
}

void ProcessCommand(Node &current, std::string &input) {
    // Remove $ char
    input = input.substr(input.find(' ') + 1);
    // Get command
    auto command = input.substr(0, input.find(' '));
    // Ignore
    if (command.find("cd") == std::string::npos) {
        return;
    }
    auto arguments = input.substr(input.find(' ') + 1);
}

// cd <name> -> current = GetChildWithName(current, <name>)
// cd ..     -> current = current.parent
// ls        -> do nothing
// <filename> || <dirname> -> current.children.push_back(<new_node>)
void BuildFileTree(std::ifstream &description, Node &root) {
    std::string input;
    Node &current = root;
    description >> input; // Skip first line
    while (description >> input) {
        if (IsCommand(input)) {
            ProcessCommand(current, input);
        }
    }
}

int main(int argc, char *argv[]) {
    // Start file stream
    std::ifstream input_file("data.txt");
    if (!input_file) {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }

    Node root{"root"};
    std::string test_command = "$ cd somefolder";
    std::string another_test_command = "$ ls";
    ProcessCommand(root, test_command);
    ProcessCommand(root, another_test_command);
    return 0;
}