#include <format>
#include <iostream>
#include <string>
int main(int argc, char** argv) {
  std::string name = "foobar";
  int age{42};
  std::cout << std::format("Hello I am {} and {} years old !", name, age) << std::endl;
  return 0;
}
