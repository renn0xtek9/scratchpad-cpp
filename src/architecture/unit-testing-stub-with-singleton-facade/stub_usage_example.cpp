#include <iostream>

#include "csv_logger_facade_singleton.h"
#include "csv_logger_writer.h"
#include "third_party_class.h"
int main(int argc, char const* argv[]) {
  CsvLoggerFacadeSingleton<int>::setLoggerInterface(std::make_shared<CsvLoggerWriter>());
  auto singleton = CsvLoggerFacadeSingleton<int>::get();
  ThirdPartyClass third_party_class;
  third_party_class.setData(42);

  return 0;
}
