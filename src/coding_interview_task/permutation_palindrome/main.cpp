#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>
std::map<char, int> createDictOfCharacters(const std::string& input) {
  std::map<char, int> output{};
  for (auto& val : input) {
    if (output.find(val) != output.end()) {
      output[val]++;
    } else {
      output[val] = 1;
    }
  }
  return output;
}

bool isPalindrome(const std::string& input) {
  const auto dict = createDictOfCharacters(input);
  for (const auto& key_value_pair : dict) {
    if (key_value_pair.second % 2 == 1) {
      // treat the middle characters
      if (input.size() % 2 == 1) {
        const auto middle_character = input.at(static_cast<int>(input.size()) / 2);
        if (key_value_pair.first == middle_character) {
          continue;
        }
      }
      return false;
    }
  }
  return true;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> inputs{{"civic"}, {"foobar"}};
  for (const auto& input : inputs) {
    std::cout << " Input: " << input << " is a palindrome ? " << isPalindrome(input) << std::endl;
  }

  std::cout << "Permuation palindrome: END " << std::endl;
  return 0;
}