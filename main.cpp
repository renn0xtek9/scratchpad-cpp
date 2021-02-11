#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <exception>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <numeric>
#include <thread>
#include <tuple>
#include <vector>

#include <modules.h>



int main() {

  Two two;

  std::function<void (Two,const PodTwo)> callback=&Two::triggerTwo;
  // callback(two,PodTwo{});
  OneA one_a(two,&Two::triggerTwo);
  one_a.setPod(PodOne{});


  return 0;
}
