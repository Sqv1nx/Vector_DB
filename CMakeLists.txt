cmake_minimum_required(VERSION 3.15)
project(Vector_DB)

# Require C++20 as the standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Make the include directory available for headers
include_directories(include)

# Compile the executable using our source files
add_executable(Vector_DB 
    src/main.cpp 
    src/flat_search.cpp
)
