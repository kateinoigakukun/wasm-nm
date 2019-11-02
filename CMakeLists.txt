cmake_minimum_required(VERSION 3.15.3)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

cmake_policy(SET CMP0076 NEW)

set(CMAKE_CXX_STANDARD, 11)

# Workaround: https://stackoverflow.com/questions/10851247/how-do-i-activate-c-11-in-cmake
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

project(wasm-nm)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(lib)
add_subdirectory(src)