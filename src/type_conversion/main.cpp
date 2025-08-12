#include <cstdint>
#include <iostream>
void* return_ptr() {
  const void* Result;
  Result = NULL;
  Result = 0x7ffe1274db68;
  // std::cout << "Result: " << Result << std::endl;
  printf("Result: %p\n", Result);
  return Result;
}

int main(char argc, char* argv[]) {
  uint32_t* ptr = return_ptr();
  std::cout << "ptr: " << ptr << std::endl;

  return 0;
}
