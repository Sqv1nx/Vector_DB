cmake_minimum_required(VERSION 3.15)
project(Vector_DB LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(vectordb_core
    src/flat_search.cpp
)
target_include_directories(vectordb_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# CLI executable
add_executable(Vector_DB
    src/main.cpp
)
target_link_libraries(Vector_DB PRIVATE vectordb_core)

# Tests (wire up once test sources exist under tests/)
enable_testing()
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt)
    add_subdirectory(tests)
endif()