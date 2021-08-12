cmake_minimum_required(VERSION 3.19)
project(Scratchpad)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(main main.cpp)
target_link_libraries(main pthread)
