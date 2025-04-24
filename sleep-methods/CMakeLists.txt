my_add_executable(CPP_STANDARD 17 SRCS main.cpp utils.cpp)

set(CMAKE_SLEEP_TIME_NOMINAL 0.0005)

target_link_libraries(sleep-methods -lstdc++fs)
target_include_directories(sleep-methods PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/plot_results.gp.in"
               "${CMAKE_CURRENT_BINARY_DIR}/plot_results.gp")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/definitions.h.in"
               "${CMAKE_CURRENT_BINARY_DIR}/definitions.h")

install(PROGRAMS "${CMAKE_CURRENT_BINARY_DIR}/plot_results.gp"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/share/sleep-methods")
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/Report.md"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/share/sleep-methods")
