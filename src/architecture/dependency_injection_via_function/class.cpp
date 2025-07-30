#include "class.h"

#include <iostream>

MyClass::MyClass(std::function<int(void)> injected_dependency) : injected_dependency_(injected_dependency) {
}

void MyClass::printSomething() {
  int result = injected_dependency_();
  std::cout << "Injected dependency returned: " << result << std::endl;
}
