#include <classes.h>

#include <iostream>

int main(int argc, char** argv) {
  std::cout << "Slicing Example" << std::endl;
  {
    std::cout << "Creating BaseClassWithoutVirtualDestructor instance" << std::endl;
    BaseClassWithoutVirtualDestructor base{};
  }
  {
    std::cout << "Creating DerivedClassWithOwnDestructor instance" << std::endl;
    DerivedClassWithOwnDestructor derived{};
  }
  {
    std::cout << "Creating BaseClassWithoutVirtualDestructor pointer to DerivedClassWithOwnDestructor" << std::endl;
    BaseClassWithoutVirtualDestructor* basePtr = new DerivedClassWithOwnDestructor();
    delete basePtr;  // This will not call the destructor of DerivedClassWithOwnDestructor
  }
  return 0;
}
