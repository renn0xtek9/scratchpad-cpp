#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct BasketsInfo {
  std::unordered_map<std::string, int> basket;
  int number_of_fruits{};
};

int main(int argc, char* argv[]) {
  std::vector<std::string> inputs{"apple", "orange", "coconut", "apple", "coconut", "foobar"};

  std::vector<BasketsInfo> parsed_baskets{};
  for (auto it = inputs.begin(); it < inputs.end(); it = std::next(it, 1)) {
    parsed_baskets.push_back(BasketsInfo{});
    bool end_of_basket{false};
    for (auto sub_it = it; sub_it < inputs.end(); sub_it = std::next(sub_it, 1)) {
      parsed_baskets.back().basket[*sub_it]++;
      if (parsed_baskets.back().basket.size() == 3) {
        end_of_basket = true;
        break;
      }
      parsed_baskets.back().number_of_fruits++;
    }
  }
  auto biggest_basket_it = std::max_element(
      parsed_baskets.begin(), parsed_baskets.end(),
      [](const BasketsInfo& lhs, const BasketsInfo& rhs) { return lhs.number_of_fruits < rhs.number_of_fruits; });

  for (auto it = std::next(biggest_basket_it->basket.begin()); it != biggest_basket_it->basket.end(); ++it) {
    std::cout << "Fruit: " << it->first << " Number " << it->second << std::endl;
  }
  std::cout << "Fruit basket: END" << std::endl;
  return 0;
}