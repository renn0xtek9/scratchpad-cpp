#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
std::string extractSubstringWithKDistinctFromIndex(const std::string& input,
                                                   const std::uint32_t k,
                                                   const std::size_t start_index) {
  std::string substring{input.at(start_index)};
  std::uint32_t distinct_encountered{};
  for (std::size_t index = start_index + 1U; index < input.size(); ++index) {
    if (substring.find(input.at(index)) == std::string::npos) {
      distinct_encountered++;
      if (distinct_encountered >= k) {
        return substring;
      }
    }
    substring.push_back(input.at(index));
  }
  return substring;
}

std::string extractLongestSubstringOfKDistinct(const std::string& input, const std::uint32_t k) {
  std::string longest_substring{input.at(0)};
  for (std::size_t index = 0U; index < input.size(); ++index) {
    const auto substring = extractSubstringWithKDistinctFromIndex(input, k, index);
    if (substring.size() > longest_substring.size()) {
      longest_substring = substring;
    }
  }
  return longest_substring;
}

int main(int argc, char* argv[]) {
  bool all_okay{true};

  if (extractLongestSubstringOfKDistinct("araaci", 1) != std::string{"aa"}) {
    std::cout << "Wrong output for k=1: got" << extractLongestSubstringOfKDistinct("araaci", 1) << std::endl;
    all_okay = false;
  }
  if (extractLongestSubstringOfKDistinct("araaci", 2) != std::string{"araa"}) {
    std::cout << "Wrong output for k=2: got" << extractLongestSubstringOfKDistinct("araaci", 2) << std::endl;
    all_okay = false;
  }
  if (extractLongestSubstringOfKDistinct("araaci", 3) != std::string{"araac"}) {
    std::cout << "Wrong output for k=3: got" << extractLongestSubstringOfKDistinct("araaci", 3) << std::endl;
    all_okay = false;
  }

  if (all_okay) {
    std::cout << "SUCCESS" << std::endl;
  } else {
    std::cout << "FAILED" << std::endl;
  }
  std::cout << "Coding Task: Longest substring of k distinct character" << std::endl;
  return 0;
}
