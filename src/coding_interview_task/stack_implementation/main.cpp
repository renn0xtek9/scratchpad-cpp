#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>

template <typename T>
class Stack {
 public:
  void push(const T&);
  T pop();
  T top() const;
  T min() const;

 private:
  std::queue<T> queue{};
  T global_min{std::numeric_limits<T>::max()};
};

template <typename T>
void Stack<T>::push(const T& value) {
  global_min = std::min(value, global_min);
  queue.push(value);
}

template <typename T>
T Stack<T>::pop() {
  const T value{queue.front()};
  queue.pop();
  return value;
}

template <typename T>
T Stack<T>::top() const {
  return queue.front();
}

template <typename T>
T Stack<T>::min() const {
  return global_min;
}

int main(int argc, char* argv[]) {
  Stack<int> unit{};

  unit.push(2);
  unit.push(1);

  if (unit.min() != 1) {
    std::cout << "Min wrong: got" << unit.min() << std::endl;
  }
  if (unit.top() != 2) {
    std::cout << "Top wrong: got" << unit.top() << std::endl;
  }
  const auto popped = unit.pop();
  if (popped != 2) {
    std::cout << "Popped wrong: got" << popped << std::endl;
  }

  std::cout << "Coding Task stack Implementation: END" << std::endl;
  return 0;
}