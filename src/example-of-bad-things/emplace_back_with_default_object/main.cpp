#include <payload.h>

#include <iostream>
#include <vector>

int main(int argc, char** argv) {
  std::cout << "---------------" << std::endl
            << "Wrong: emplace_back(MyClass{})" << std::endl
            << "---------------" << std::endl;
  std::vector<MyClass> vector1{};
  constexpr int number_of_elements{3};
  vector1.reserve(number_of_elements);
  for (std::size_t index = 0; index < number_of_elements; ++index) {
    std::cout << "Index " << index << std::endl;
    fflush(stdout);
    vector1.push_back(MyClass{});
  }
  std::cout << "---------------" << std::endl
            << "Right: with emplace_back()" << std::endl
            << "---------------" << std::endl;

  std::vector<MyClass> vector2{};
  vector2.reserve(number_of_elements);
  for (std::size_t index = 0; index < number_of_elements; ++index) {
    std::cout << "Index " << index << std::endl;
    fflush(stdout);
    vector2.emplace_back();
  }

  std::cout << "Example: payload without move constructor: END" << std::endl;
  fflush(stdout);
}
