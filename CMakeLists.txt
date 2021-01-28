add_executable(main main.cpp)
target_link_libraries(main pthread)
target_compile_options(main PRIVATE -Wall -Wextra -pedantic -Werror)