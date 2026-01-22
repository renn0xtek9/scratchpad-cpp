#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

int ComputeMaximumReturn(const std::vector<int>& stock_prices) {
  int min_gain = std::numeric_limits<int>::min();

  for (auto buy_it = stock_prices.begin(); buy_it != std::prev(stock_prices.end()); ++buy_it) {
    auto max_sell_it = std::max_element(std::next(buy_it, 1), stock_prices.end());
    auto gain = *max_sell_it - *buy_it;
    if (gain > min_gain) {
      min_gain = gain;
    }
  }
  return min_gain;
}

int main(int argc, char* argv[]) {
  std::vector<int> stock_prices{10, 7, 5, 8, 11, 9};

  auto result = ComputeMaximumReturn(stock_prices);
  std::cout << "Maximum expected return " << result << std::endl;

  std::cout << "Coding Task: Apple stocks: END" << std::endl;
  return 0;
}
