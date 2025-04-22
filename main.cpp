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
#include <fstream>

#include "dummy_classes.h"

int main()
{
  std::size_t number_of_bytes = 1024U * 1024U;

  std::cout << "This programm will allocate " << number_of_bytes << " bytes." << std::endl;

  std::vector<char> vector{};
  vector.reserve(number_of_bytes);

  for (int i = 0; i < number_of_bytes; ++i)
  {
    std::cout << "Allocating byte no: " << i << " out of " << number_of_bytes << std::endl;
    vector.push_back('a');
  }

  std::cout << "Success !!" << std::endl;

  return 0;
}
