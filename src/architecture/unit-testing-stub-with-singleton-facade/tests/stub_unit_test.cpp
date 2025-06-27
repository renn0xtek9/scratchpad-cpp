#include <iostream>

#include "../csv_logger_facade_singleton.h"
#include "../csv_logger_mock.h"
#include "../third_party_class.h"
int main(int argc, char const* argv[]) {
  auto mock_writer = std::make_shared<CsvLoggerMock>();
  CsvLoggerFacadeSingleton<int>::setLoggerInterface(mock_writer);
  auto singleton = CsvLoggerFacadeSingleton<int>::get();
  ThirdPartyClass third_party_class;
  third_party_class.setData(42);

  return 0;
}
