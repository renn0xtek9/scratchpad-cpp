#include <functional>
#ifndef CLASS_H
#define CLASS_H
class MyClass {
 public:
  MyClass(std::function<int(void)> injected_dependency);

  void printSomething();

 private:
  std::function<int(void)> injected_dependency_;
};

#endif  // CLASS_H
