#include <log_interface_double.h>
#include <log_interface_int.h>

#include <iostream>

void LogInterfaceint::log(const int& data) {
  std::cout << "Logging int data: " << data << std::endl;
}

void LogInterfacedouble::log(const double& data) {
  std::cout << "Logging double data: " << data << std::endl;
}
