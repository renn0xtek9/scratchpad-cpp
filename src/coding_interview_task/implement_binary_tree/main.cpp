#include <iostream>
#include <memory>
class BinaryTreeNode {
 public:
  using ChildNode = std::unique_ptr<BinaryTreeNode>;
  BinaryTreeNode() {
  }
  BinaryTreeNode(ChildNode left_child, ChildNode right_child)
      : left(std::move(left_child)), right(std::move(right_child)) {
  }
  ~BinaryTreeNode() = default;
  BinaryTreeNode(const BinaryTreeNode& other) = delete;
  BinaryTreeNode(BinaryTreeNode&& other) = default;
  BinaryTreeNode& operator=(const BinaryTreeNode& rhs) = delete;
  BinaryTreeNode& operator=(BinaryTreeNode&& rhs) = default;
  ChildNode left{};
  ChildNode right{};
};

int main(int argc, char* argv[]) {
  auto node_a =
      std::make_unique<BinaryTreeNode>(std::make_unique<BinaryTreeNode>(), std::make_unique<BinaryTreeNode>());
  auto node_b =
      std::make_unique<BinaryTreeNode>(std::make_unique<BinaryTreeNode>(), std::make_unique<BinaryTreeNode>());
  auto root_node = std::make_unique<BinaryTreeNode>(std::move(node_a), std::move(node_b));

  std::cout << "Coding Task: implement a binary tree in C++: END" << std::endl;
  return 0;
}
