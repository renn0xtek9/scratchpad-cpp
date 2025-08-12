# This function simplifies the creation of C++ executables in a CMake project.
# It parses arguments to configure the executable's name, source files, C++
# standard, compile flags, and installation properties.
#
# Arguments: NAME (optional): The name of the executable. Defaults to the
# directory name if not provided. CPP_STANDARD (optional): The C++ standard to
# use (e.g., 14, 17, 20). Defaults to 14. SRCS (optional): A list of source
# files for the executable. Defaults to "main.cpp". COMPILE_FLAGS (optional):
# Additional compile flags for the executable.
#
# Example usage: my_add_executable( NAME my_app CPP_STANDARD 17 SRCS main.cpp
# utils.cpp COMPILE_FLAGS "-Wall;-Wextra" )
function (my_add_executable)

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

  if (NOT arg_CPP_STANDARD)
    set (arg_CPP_STANDARD 14)
  endif (NOT arg_CPP_STANDARD)

  if (NOT arg_SRCS)
    set (arg_SRCS main.cpp)
  endif (NOT arg_SRCS)

  if (NOT arg_NAME)
    get_filename_component (DIR_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
    set (executable_name_ ${DIR_NAME})
  else (NOT arg_NAME)
    set (executable_name_ ${arg_NAME})
  endif (NOT arg_NAME)

  add_executable (${executable_name_} ${arg_SRCS})
  target_link_libraries (${executable_name_} pthread)

  target_compile_options (
    ${executable_name_} PRIVATE $<$<CXX_COMPILER_ID:GNU>:${arg_COMPILE_FLAGS}>)

  set_target_properties (
    ${executable_name_} PROPERTIES CXX_STANDARD ${arg_CPP_STANDARD}
                                   CXX_STANDARD_REQUIRED ON)

  install (
    TARGETS ${executable_name_}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

endfunction ()
