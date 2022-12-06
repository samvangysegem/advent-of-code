cmake_minimum_required(VERSION 3.24)
project(advent_of_code)

# Lists
set(FOLDERS "D1" "D2" "D3" "D4" "D5" "D6")

# Target folder
SET(FOLDER "D1" CACHE STRING "Choose target folder to compile")
if (FOLDER IN_LIST FOLDERS)
    message("Compiling folder " ${DAY})
else()
    message( FATAL_ERROR "Specified target folder is unkown!")
endif()

# Defines interface library that specifies the language standard
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

# Input file
configure_file(${CMAKE_SOURCE_DIR}/${FOLDER}/input/data.txt ${CMAKE_BINARY_DIR}/data.txt)

# Add executable target
add_executable(solution ${FOLDER}/src/main.cpp)                                               
target_include_directories(solution PUBLIC ${FOLDER}/include)                                      
target_link_libraries(solution PRIVATE project_options)


