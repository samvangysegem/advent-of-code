cmake_minimum_required(VERSION 3.24)

# Project settings
project(aoc_day2)
message("Build type: " $(CMAKE_BUILD_TYPE))

# Defines interface library that specifies the language standard
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

# Input file
configure_file(${CMAKE_SOURCE_DIR}/input/aoc_day2.txt ${CMAKE_BINARY_DIR}/aoc_day2.txt)

# Add executable target
add_executable(challenge src/main.cpp)                                               
target_include_directories(challenge PUBLIC include)                                      
target_link_libraries(challenge PRIVATE project_options)


