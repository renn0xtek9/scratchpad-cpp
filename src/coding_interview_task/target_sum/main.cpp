#include <algorithm>
#include <iostream>
#include <optional>
#include <utility>
#include <vector>

std::optional<std::pair<std::size_t, std::size_t>> findPair(const std::vector<int>& input, const int target_sum) {
  std::optional<std::pair<std::size_t, std::size_t>> result{};
  for (auto it = input.begin(); it != input.end(); ++it) {
    const auto complement{target_sum - *it};
    const auto it_of_second = std::find(std::next(it, 1), input.end(), complement);
    if (it_of_second != input.end()) {
      result = std::make_pair<std::size_t, std::size_t>(std::distance(input.begin(), it),
                                                        std::distance(input.begin(), it_of_second));
      return result;
    }
  }
  return result;
}

int main(int argc, char* argv[]) {
  std::vector<int> input{1, 2, 6, 8, 16, 26};

  auto result = findPair(input, 14);

  if (result.has_value()) {
    std::cout << "Pair of index: " << result->first << " " << result->second << std::endl;
    if (result->first == 2 && result->second == 3) {
      std::cout << "SUCCESS" << std::endl;
    } else {
      std::cout << "FAILED" << std::endl;
    }
  } else {
    std::cout << "FAILED" << std::endl;
  }

  std::cout << "Coding Task: Target sum: END" << std::endl;
  return 0;
}
