#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <exception>
#include <functional>
#include <future>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <numeric>
#include <thread>
#include <tuple>
#include <vector>

using namespace std::literals::chrono_literals;

class LocalPoseCalculation {
public:
  LocalPoseCalculation() = default;
  virtual ~LocalPoseCalculation() = default;

  LocalPoseCalculation(const LocalPoseCalculation &) = default;
  LocalPoseCalculation(LocalPoseCalculation &&) = default;

  LocalPoseCalculation &operator=(const LocalPoseCalculation &) = default;
  LocalPoseCalculation &operator=(LocalPoseCalculation &&) = default;

  virtual void updateAccelerationAndAngularRate(const int foo) = 0;
};

class LocalPoseCalculationFromGlobalPose final : public LocalPoseCalculation {
public:
  explicit LocalPoseCalculationFromGlobalPose(){};

private:
  void updateAccelerationAndAngularRate(const int foo) override{};
};

int main() {

  std::unique_ptr<LocalPoseCalculation> local_pose_calc_ =
      std::make_unique<LocalPoseCalculationFromGlobalPose>();

  //Uncommenting the later won't compile
  // local_pose_calc_->updateAccelerationAndAngularRate();

  std::cout << "End" << std::endl;

  return 0;
}
