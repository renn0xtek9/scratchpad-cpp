#include <algorithm>
#include <iostream>
#include <set>
#include <tuple>
#include <vector>

using output_type = std::set<std::tuple<int, int, int>>;

void display_output(const output_type& output) {
  std::cout << "Output triplets summing to zero:\n";
  for (const auto& triplet : output) {
    std::cout << "(" << std::get<0>(triplet) << ", " << std::get<1>(triplet) << ", " << std::get<2>(triplet) << ")\n";
  }
}

int main(int argc, char* argv[]) {
  std::vector<int> input{0, 1, 2, 3, -1, 0, 1, -1};
  output_type output;
  // expected std::vector<std::tuple<int,int,int> output{std::make_tuple(0,1,-1)};

  for (auto it_a = input.begin(); it_a != input.end(); ++it_a) {
    for (auto it_b = std::next(it_a); it_b != input.end(); ++it_b) {
      for (auto it_c = std::next(it_b); it_c != input.end(); ++it_c) {
        if ((*it_a + *it_b + *it_c) == 0) {
          output.insert({*it_a, *it_b, *it_c});
        }
      }
    }
  }

  display_output(output);
  std::cout << "Coding Task: triplet of integer summing to zero: END" << std::endl;
  return 0;
}