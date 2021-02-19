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



// void wireTogether(std::pair<std)


// connect(OneA,

int main() {

  PodOne pod_one;
  pod_one.timestamp=-12;

  Two two;
  PodOne pod;
  std::cout<<pod.isValid();

  // std::function<void (Two,const PodTwo)> callback=&Two::triggerTwo;
  OneA one_a(two,&Two::triggerTwo);
  one_a.setPod(PodOne{});


  return 0;
}
