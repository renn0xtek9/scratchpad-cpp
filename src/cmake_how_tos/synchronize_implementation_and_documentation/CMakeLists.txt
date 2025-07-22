include (parameter_list.cmake)

configure_file (Requirements.md.in Requirements.md @ONLY)
configure_file (main.cpp.in main.cpp @ONLY)
my_add_executable (code_generation_of_document main.cpp)
