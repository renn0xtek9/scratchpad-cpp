#ifndef CSV_LOGGER_MOCK_H
#define CSV_LOGGER_MOCK_H
#include <iostream>

#include "csv_logger_interface.h"
class CsvLoggerMock : public CsvLoggerInterface<int> {
 public:
  CsvLoggerMock() = default;
  ~CsvLoggerMock() override = default;

  // Mock implementation of initHeader
  void initHeader() override {
    std::cout << "Mock CSV Logger implementation - : Initializing CSV header." << std::endl;
    init_called_ = true;
  }

  // Mock implementation of logLine
  void logLine(int data) override {
    std::cout << "Mock CSV Logger implementation - : Logging data: " << data << std::endl;
    log_called_ = true;
  }

  bool getInitCalled() const {
    return init_called_;
  }
  bool getLogCalled() const {
    return log_called_;
  }

 private:
  bool init_called_{false};
  bool log_called_{false};
};
#endif  // CSV_LOGGER_MOCK_H
