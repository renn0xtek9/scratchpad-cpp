#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <exception>
#include <functional>
#include <future>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <numeric>
#include <thread>
#include <tuple>
#include <vector>

#include "dummy_classes.h"

int main() {
  StringRuleOfFive item;
  item.print();

  std::cout << "End" << std::endl;

  return 0;
}
