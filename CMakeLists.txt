cmake_minimum_required(VERSION 3.19)
project(Scratchpad)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)



add_library(my_lib STATIC src/my_lib.cpp)
target_include_directories(my_lib
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:new_install/include>)


# target_precompile_headers(my_lib PUBLIC include/my_lib.h)
add_executable(main main.cpp)
target_link_libraries(main PRIVATE my_lib)
# target_compile_options(main PRIVATE -Wall -Wextra -pedantic -Werror)
