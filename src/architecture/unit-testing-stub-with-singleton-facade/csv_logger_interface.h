#ifndef CSV_LOGGER_INTERFACE_H
#define CSV_LOGGER_INTERFACE_H
template <typename Data>
class CsvLoggerInterface {
 public:
  virtual ~CsvLoggerInterface() = default;

  // Pure virtual function to initialize the header
  virtual void initHeader() = 0;

  // Pure virtual function to log data
  virtual void logLine(Data data) = 0;
};

#endif  // CSV_LOGGER_INTERFACE_H
