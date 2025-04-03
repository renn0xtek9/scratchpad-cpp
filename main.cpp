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

int main()
{
  std::unique_ptr<StringRuleOfFive> str_rule_of_five;
  str_rule_of_five->print(); //works but should not ! is it because the print function is inlined ?
  str_rule_of_five->add_one_to_a();  //Thins segfault as it should !

  std::cout
      << "End" << std::endl;

  return 0;
}
