include (parameter_list.cmake)

configure_file (Requirements.md.in Requirements.md @ONLY)
configure_file (main.cpp main.cpp.generated @ONLY)
add_executable (synchronize_implementation_and_documentation main.cpp.generated)
set_source_files_properties (main.cpp.generated PROPERTIES LANGUAGE CXX)
