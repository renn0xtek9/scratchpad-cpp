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

  set (CMAKE_CXX_STANDARD ${arg_CPP_STANDARD})
  set (CMAKE_CXX_STANDARD_REQUIRED True)
  if (NOT arg_NAME)
    get_filename_component (DIR_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
    set (EXECUTABLE_NAME ${DIR_NAME})
  else (NOT arg_NAME)
    set (EXECUTABLE_NAME ${arg_NAME})
  endif (NOT arg_NAME)

  add_executable (${EXECUTABLE_NAME} ${arg_SRCS})
  target_link_libraries (${EXECUTABLE_NAME} pthread)

  target_compile_options (
    ${EXECUTABLE_NAME} PRIVATE $<$<CXX_COMPILER_ID:GNU>:${arg_COMPILE_FLAGS}>)

  install (
    TARGETS ${EXECUTABLE_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

endfunction ()
