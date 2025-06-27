#ifndef CSV_LOGGER_WRITER_H
#define CSV_LOGGER_WRITER_H

#include <iostream>

#include "csv_logger_interface.h"

class CsvLoggerWriter : public CsvLoggerInterface<int> {
 public:
  CsvLoggerWriter() = default;
  ~CsvLoggerWriter() override = default;

  // Implementation of initHeader
  void initHeader() override {
    std::cout << "Actual CSV Logger implementation - Initializing CSV header." << std::endl;
    // Here you would typically write the header to a CSV file.
  }

  // Implementation of logLine
  void logLine(int data) override {
    std::cout << "Actual CSV Logger implementation - Logging data: " << data << std::endl;
    // Here you would typically write the data to a CSV file.
  }
};

#endif  // CSV_LOGGER_WRITER_H
