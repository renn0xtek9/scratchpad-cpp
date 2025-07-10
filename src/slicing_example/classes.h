#ifndef CLASSES_H
#define CLASSES_H
#include <iostream>
#include <string>
const std::string green = "\033[32m";
const std::string red = "\033[31m";
const std::string reset = "\033[0m";

std::string constructorString() {
  return std::string{green} + " constructor " + std::string{reset};
}

std::string destructorString() {
  return std::string{red} + " destructor " + std::string{reset};
}

class BaseClassWithoutVirtualDestructor {
 public:
  BaseClassWithoutVirtualDestructor() {
  }

  ~BaseClassWithoutVirtualDestructor() {
    std::cout << "BaseClassWithoutVirtualDestructor " << destructorString() << " called" << std::endl;
  }

  BaseClassWithoutVirtualDestructor(const BaseClassWithoutVirtualDestructor& other) = delete;
  BaseClassWithoutVirtualDestructor(BaseClassWithoutVirtualDestructor&& other) = delete;

  BaseClassWithoutVirtualDestructor& operator=(const BaseClassWithoutVirtualDestructor& other) = delete;
  BaseClassWithoutVirtualDestructor& operator=(BaseClassWithoutVirtualDestructor&& other) = delete;
};

class DerivedClassWithOwnDestructor : public BaseClassWithoutVirtualDestructor {
 public:
  DerivedClassWithOwnDestructor() {
    std::cout << "DerivedClassWithOwnDestructor " << constructorString() << " called" << std::endl;
  }

  ~DerivedClassWithOwnDestructor() {
    std::cout << "DerivedClassWithOwnDestructor " << destructorString() << " called" << std::endl;
    std::cout << "\tDeleting derived_ptr_: " << *derived_ptr << std::endl;
    delete derived_ptr;
  }

 private:
  int* derived_ptr = new int(84);  // Own resource to demonstrate slicing
};

#endif  // CLASSES_H
