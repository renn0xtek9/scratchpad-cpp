my_add_unit_test (
  NAME
  stub_unit_coverage_test
  CPP_STANDARD_14
  SRCS
  stub_unit_test.cpp
  ../third_party_class_stub.cpp
  ../csv_logger_facade_singleton.cpp
  ../csv_logger_interface.h
  ../csv_logger_mock.h
  ../csv_logger_writer.h)

my_add_unit_test (
  NAME csv_logger_facade_singleton_test CPP_STANDARD_14 SRCS
  csv_logger_facade_singleton_test.cpp ../csv_logger_facade_singleton.cpp)
