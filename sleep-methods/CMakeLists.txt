my_add_executable(CPP_STANDARD 17 SRCS main.cpp utils.cpp)
target_link_libraries(sleep-methods -lstdc++fs)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/plot_results.gp.in"
               "${CMAKE_CURRENT_BINARY_DIR}/plot_results.gp")
install(PROGRAMS "${CMAKE_CURRENT_BINARY_DIR}/plot_results.gp"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/share/sleep-methods")
