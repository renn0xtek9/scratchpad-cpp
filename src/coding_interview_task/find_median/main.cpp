#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

class MedianFinder {
 public:
  void Insert(int num) {
    values.emplace_back(num);
    std::sort(values.begin(), values.end());
  }
  double FindMedian() const {
    double result{};
    if (values.empty()) {
      return 0.0;
    }
    if (values.size() % 2U == 0) {
      const auto total = std::accumulate(values.begin(), values.end(), 0);
      result = static_cast<double>(total) / static_cast<double>(values.size());
    } else {
      const auto middle_index = std::floor(values.size() / 2U);
      result = values.at(middle_index);
    }
    return result;
  }

 private:
  std::vector<int> values{};
};

int main(int argc, char* argv[]) {
  MedianFinder median_finder{};

  median_finder.Insert(1);
  if (median_finder.FindMedian() != 1.0) {
    std::cout << "Failed: got " << median_finder.FindMedian() << " expected 1" << std::endl;
  }

  median_finder.Insert(6);

  if (median_finder.FindMedian() != 3.5) {
    std::cout << "Failed: got " << median_finder.FindMedian() << " expected 3.5" << std::endl;
  }

  median_finder.Insert(2);

  if (median_finder.FindMedian() != 2.0) {
    std::cout << "Failed: got " << median_finder.FindMedian() << " expected 2.0" << std::endl;
  }

  median_finder.Insert(5);

  if (median_finder.FindMedian() != 3.5) {
    std::cout << "Failed: got " << median_finder.FindMedian() << " expected 3.5" << std::endl;
  }

  std::cout << "Coding Task: Find Median: END" << std::endl;
  return 0;
}
