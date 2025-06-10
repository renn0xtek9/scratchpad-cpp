my_add_executable (
  NAME
  stub_usage_example
  CPP_STANDARD_14
  SRCS
  stub_usage_example.cpp
  third_party_class_stub.cpp
  csv_logger_facade_singleton.cpp
  csv_logger_interface.h
  csv_logger_mock.h
  csv_logger_writer.h)

# Smoke test to make sure the stub works.
add_test (NAME stub_usage_example COMMAND stub_usage_example)

add_subdirectory (tests)
