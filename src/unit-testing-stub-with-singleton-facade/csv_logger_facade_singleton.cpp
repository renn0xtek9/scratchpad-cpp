#include "csv_logger_facade_singleton.h"
template <>
std::unique_ptr<CsvLoggerFacadeSingleton<int>> CsvLoggerFacadeSingleton<int>::instance = nullptr;
template <>
std::shared_ptr<CsvLoggerInterface<int>> CsvLoggerFacadeSingleton<int>::csv_logger_interface_ = nullptr;
