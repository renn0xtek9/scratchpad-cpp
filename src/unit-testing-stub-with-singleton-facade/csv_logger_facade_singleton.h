#ifndef CSV_LOGGER_FACADE_SINGLETON_H
#define CSV_LOGGER_FACADE_SINGLETON_H
#include <memory>

#include "csv_logger_interface.h"
#include "csv_logger_mock.h"
#include "csv_logger_writer.h"

/*! @brief Singleton facade for CsvLoggerInterface

@methods:
- setLoggerInterface: Sets the default logger interface to be used by the singleton.
- get: Returns the singleton instance of CsvLoggerFacadeSingleton.
- getLoggerInterface: Returns the logger interface set in the singleton.
@tparam Data: The type of data to be logged.
@details:
This class provides a singleton facade for the CsvLoggerInterface, allowing for easy access to logging functionality
throughout the application. The singleton facade must first be initialized by calling setLoggerInteface. Then it can be
called from anywhere using the get() method. One can access the logger interface using getLoggerInterface() method.

@note:
The main usecase for this singleton is to provide a way of doing dependency injection of the CsvLogger in stubs where
the singleton is used. This is usefull to inject mock of csv logger in unit tests of stubs.
*/
template <typename Data>
class CsvLoggerFacadeSingleton {
 public:
  CsvLoggerFacadeSingleton() = default;
  ~CsvLoggerFacadeSingleton() = default;

  static void setLoggerInterface(std::shared_ptr<CsvLoggerInterface<Data>> default_logger_interface) {
    if (csv_logger_interface_ != default_logger_interface) {
      csv_logger_interface_ = default_logger_interface;
    }

    if (!csv_logger_interface_) {
      throw std::runtime_error("CsvLoggerFacadeSingleton cannot be initializat without valid CsvLoggerInterface.");
    }
  }

  static CsvLoggerFacadeSingleton& get() {
    if (!csv_logger_interface_) {
      throw std::runtime_error("Called get() before setLoggerInterface().");
    }
    if (!instance) {
      instance = std::make_unique<CsvLoggerFacadeSingleton<Data>>();
    }
    return *instance.get();
  }

  CsvLoggerInterface<Data>& getLoggerInterface() {
    return *csv_logger_interface_;
  }

 private:
  static std::unique_ptr<CsvLoggerFacadeSingleton<Data>> instance;
  static std::shared_ptr<CsvLoggerInterface<Data>> csv_logger_interface_;
};

#endif  // CSV_SINGLETON_H
