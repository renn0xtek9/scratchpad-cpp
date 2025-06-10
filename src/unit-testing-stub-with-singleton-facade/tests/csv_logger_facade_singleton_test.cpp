#include "../csv_logger_facade_singleton.h"

#include <cassert>

struct DummyData {};

class DummyMock : public CsvLoggerInterface<DummyData> {
 public:
  DummyMock() {
    id++;
  };
  ~DummyMock() override = default;

  // Mock implementation of initHeader
  void initHeader() override {
    std::cout << "DummyMock - : Initializing CSV header." << std::endl;
  }

  // Mock implementation of logLine
  void logLine(DummyData data) override {
    std::cout << "DummyMock - : Logging data " << std::endl;
  }
  static int id;
};

int DummyMock::id = 0;

template <>
std::unique_ptr<CsvLoggerFacadeSingleton<DummyData>> CsvLoggerFacadeSingleton<DummyData>::instance = nullptr;
template <>
std::shared_ptr<CsvLoggerInterface<DummyData>> CsvLoggerFacadeSingleton<DummyData>::csv_logger_interface_ = nullptr;

int main(int argc, char const* argv[]) {
  try {
    CsvLoggerFacadeSingleton<DummyData>::setLoggerInterface({});
    exit(1);  // Test fails if this line is reached.
  } catch (const std::runtime_error& e) {
    std::cout << "Caught expected exception: " << e.what() << std::endl;
  }

  try {
    auto facade_without_default = CsvLoggerFacadeSingleton<DummyData>::get();
    exit(1);  // Test fails if this line is reached.
  } catch (const std::runtime_error& e) {
    std::cout << "Caught expected exception: " << e.what() << std::endl;
  }

  auto dummy_writer = std::make_shared<DummyMock>();

  CsvLoggerFacadeSingleton<DummyData>::setLoggerInterface(dummy_writer);

  auto facade = CsvLoggerFacadeSingleton<DummyData>::get();

  facade.getLoggerInterface().initHeader();
  facade.getLoggerInterface().logLine({});
  assert(dummy_writer->id == 1);

  auto facade2 = CsvLoggerFacadeSingleton<DummyData>::get();
  assert(dummy_writer->id == 1);  // Ensure the same instance is used

  auto facade3 = CsvLoggerFacadeSingleton<DummyData>::get();
  assert(dummy_writer->id == 1);  // Ensure the same instance is used
  facade3.getLoggerInterface().logLine({});
  assert(dummy_writer->id == 1);  // Ensure the same instance is used
  facade3.getLoggerInterface().initHeader();
  assert(dummy_writer->id == 1);  // Ensure the same instance is used

  return 0;
}
