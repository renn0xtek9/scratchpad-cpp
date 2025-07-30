#include "class.h"

int main(int argc, char** argv) {
  int index = 0;

  auto function = [&index]() -> int { return ++index; };
  MyClass foo(function);
  foo.printSomething();

  MyClass bar(function);
  bar.printSomething();

  return 0;
}
