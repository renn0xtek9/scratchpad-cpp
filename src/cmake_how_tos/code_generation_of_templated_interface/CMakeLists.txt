set (LOG_INTERFACE_TYPES "int;double")
foreach (LOG_INTERFACE_TYPE IN LISTS LOG_INTERFACE_TYPES)
  string (TOUPPER "${LOG_INTERFACE_TYPE}" LOG_INTERFACE_TYPE_UPPER)
  message (
    STATUS
      "Generating header file: ${CMAKE_CURRENT_BINARY_DIR}/log_interface_${LOG_INTERFACE_TYPE}.h"
  )
  configure_file (
    "${CMAKE_CURRENT_SOURCE_DIR}/log_interface_derived.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/log_interface_${LOG_INTERFACE_TYPE}.h" @ONLY)
endforeach ()

add_library (log_interface STATIC log_interface_implementations.cpp)

target_include_directories (log_interface PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                                                  ${CMAKE_CURRENT_BINARY_DIR})
