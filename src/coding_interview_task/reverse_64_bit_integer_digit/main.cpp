#include <bitset>
#include <cstdint>
#include <iostream>

int main(int argc, char* argv[]) {
  std::int64_t number_to_reverse{123};
  std::bitset<64 * 8> bits_to_reverse(number_to_reverse);
  std::bitset<64 * 8> reversed_bits{};
  for (std::size_t index = 0; index < bits_to_reverse.size(); index++) {
    reversed_bits[reversed_bits.size() - index - 1U] = bits_to_reverse[index];
  }
  std::cout << "Integer " << number_to_reverse << std::endl;
  std::cout << "Debug: Bits to reverse " << bits_to_reverse << std::endl;
  std::cout << "Debug: Reversed bits" << reversed_bits << std::endl;

  std::cout << "Reverse 64 bit integer digit: END" << std::endl;
  return 0;
}