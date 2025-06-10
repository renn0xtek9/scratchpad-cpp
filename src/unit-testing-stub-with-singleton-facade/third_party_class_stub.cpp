#include <iostream>
#include <memory>

#include "csv_logger_facade_singleton.h"
#include "csv_logger_writer.h"
#include "third_party_class.h"

ThirdPartyClass::ThirdPartyClass() {
  std::cout << "Stub implementation of ThirdPartyClass constructor called." << std::endl;
  CsvLoggerFacadeSingleton<int>::setLoggerInterface(std::make_shared<CsvLoggerWriter>());
  auto& csv_logger = CsvLoggerFacadeSingleton<int>::get();
  csv_logger.getLoggerInterface().initHeader();
  // This is a stub implementation for the third-party library.
  // In a real scenario, this would initialize the third-party library.
  // For testing purposes, we can leave it empty or log the call.
}

void ThirdPartyClass::setData(int data) const {
  std::cout << "The stub implemation of ThirdPartyClass::setData called with data: " << data << std::endl;

  auto& csv_logger = CsvLoggerFacadeSingleton<int>::get();
  csv_logger.getLoggerInterface().logLine(data);

  // This is a stub implementation for the third-party library.
  // In a real scenario, this would interact with the third-party library.
  // For testing purposes, we can leave it empty or log the call.
}
