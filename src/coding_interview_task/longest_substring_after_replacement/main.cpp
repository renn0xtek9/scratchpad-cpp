#include <iostream>
#include <vector>

std::size_t GetLengthOfLongestSubarray(const std::vector<int>& input, const std::size_t k_replacement) {
  int longest_substring{};
  for (std::size_t start_index = 0; start_index < input.size(); ++start_index) {
    int replacement{};
    auto parse_index{start_index + 1};
    while (parse_index < input.size()) {
      if (input.at(parse_index) == 0) {
        replacement++;
      }
      if (replacement >= k_replacement) {
        break;
      }
      ++parse_index;
    }
    const auto substring_length{parse_index - start_index};
    if (longest_substring < substring_length) {
      longest_substring = substring_length;
      std::cout << "Found longer substring between index " << start_index << " and " << parse_index << std::endl;
    }
  }
  return longest_substring;
}

int main(int argc, char* argv[]) {
  std::vector<int> input{0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1};

  const auto length = GetLengthOfLongestSubarray(input, 2);
  std::cout << "Longest substring " << length << std::endl;
  std::cout << "Coding Task: Longest substring after replacement: END" << std::endl;
  return 0;
}
