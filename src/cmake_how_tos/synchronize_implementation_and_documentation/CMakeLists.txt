include (parameter_list.cmake)

configure_file (Requirements.md.in Requirements.md @ONLY)
configure_file (main.cpp.in main.cpp @ONLY)
add_executable (synchronize_implementation_and_documentation main.cpp)
