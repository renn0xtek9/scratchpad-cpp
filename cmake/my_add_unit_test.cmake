# This function simplifies the creation of C++ unit test in a CMake project. It
# parses arguments to configure the executable's name, source files, C++
# standard, compile flags, and installation properties.
#
# Arguments: NAME (optional): The name of the test. CPP_STANDARD (optional): The
# C++ standard to use (e.g., 14, 17, 20). Defaults to 14. SRCS (optional): A
# list of source files for the executable. Defaults to "main.cpp". COMPILE_FLAGS
# (optional): Additional compile flags for the executable.
#
# Example usage: my_add_unit_test( NAME my_test CPP_STANDARD 17 SRCS test.cpp
# utils.cpp COMPILE_FLAGS "-Wall;-Wextra" )
function (my_add_unit_test)
  cmake_parse_arguments (
    arg # prefix of output variables i.e. arguments will be retrieved as
    # ARG_FOO ARG_BAR
    "" # ; separated list of names of boolean arguments (only defined ones will
    # be true)
    "NAME;CPP_STANDARD" # ; separated list of names of mono-valued arguments
    "SRCS;COMPILE_FLAGS" # ; separated list of names of multi-valued arguments
    ${ARGN} # arguments of the function to parse, here we take the all original
    # ones
  )
  my_add_executable (
    NAME
    ${arg_NAME}
    CPP_STANDARD
    ${arg_CPP_STANDARD}
    SRCS
    ${arg_SRCS}
    COMPILE_FLAGS
    -fprofile-arcs
    -ftest-coverage
    -g
    -O0
    --coverage)

  target_link_options (${arg_NAME} PRIVATE $<$<CXX_COMPILER_ID:GNU>:-lgcov
                       --coverage>)

  target_link_libraries (${arg_NAME} --coverage)

  add_test (NAME ${arg_NAME} COMMAND ${arg_NAME})

  set_tests_properties (${arg_NAME} PROPERTIES FIXTURES_REQUIRED unit_tests)
  set_tests_properties (${arg_NAME} PROPERTIES LABELS unit_tests)

  if (TEST "code_coverage")
    message (DEBUG "code_coverage already exists, skipping addition")
  else ()
    add_test (
      NAME code_coverage
      COMMAND gcovr -r .. --fail-under-line 1.0 --html --html-details -o
              index.html
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    set_tests_properties (code_coverage PROPERTIES FIXTURES_CLEANUP unit_tests)
  endif ()

endfunction ()
