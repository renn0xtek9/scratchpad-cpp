#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <iostream>
class MyClass {
 public:
  MyClass() {
    std::cout << "Construtor" << std::endl;
    fflush(stdout);
  }
  ~MyClass() {
    std::cout << "Destructor" << std::endl;
    fflush(stdout);
  }
  MyClass(const MyClass& other) {
    std::cout << "Copy constructor" << std::endl;
    fflush(stdout);
  }
  MyClass(MyClass&& other) {
    std::cout << "Move constructor" << std::endl;
    fflush(stdout);
  }
  MyClass& operator=(const MyClass&) {
    std::cout << "Copy assignement" << std::endl;
    fflush(stdout);
    return *this;
  }
  MyClass& operator=(MyClass&&) {
    std::cout << "Move assignement" << std::endl;
    fflush(stdout);
    return *this;
  }
};

#endif