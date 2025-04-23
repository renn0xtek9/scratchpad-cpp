my_add_executable(CPP_STANDARD 17)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/plot_results.gp.in"
               "${CMAKE_CURRENT_BINARY_DIR}/plot_results.gp")
