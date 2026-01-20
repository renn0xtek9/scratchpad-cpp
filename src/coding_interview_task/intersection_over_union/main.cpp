#include <cstdint>
#include <iostream>

struct BoundingBox {
  std::uint16_t upper_left_x{};
  std::uint16_t upper_left_y{};
  std::uint16_t lower_right_x{};
  std::uint16_t lower_right_y{};
  std::uint16_t Area() const {
    return std::abs(upper_left_x - lower_right_x) * std::abs(upper_left_y - lower_right_y);
  }
};

float computeIntersectionArea(const BoundingBox& box1, const BoundingBox& box2) {
  auto overlap_x{std::max(0, box1.lower_right_x - box2.upper_left_x)};
  auto overlap_y{std::max(0, box1.lower_right_y - box2.upper_left_y)};

  return static_cast<float>(overlap_x * overlap_y);
}

float computeIoU(const BoundingBox& box1, const BoundingBox& box2) {
  auto intersection_area = computeIntersectionArea(box1, box2);
  std::cout << "Intersection area" << intersection_area << std::endl;
  auto union_area = box1.Area() + box2.Area();
  return intersection_area / static_cast<float>(union_area);
}

int main(int argc, char* argv[]) {
  BoundingBox box1{0, 0, 10, 10};
  BoundingBox box2{9, 9, 11, 11};

  std::cout << "IoU: " << computeIoU(box1, box2) << std::endl;

  std::cout << "Coding Task: Intersection over Union: END" << std::endl;
  return 0;
}
