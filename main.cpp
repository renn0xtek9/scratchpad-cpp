#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <exception>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <numeric>
#include <thread>
#include <tuple>
#include <vector>

// Interface
class ITrajectory {
public:
  virtual std::pair<double, double> eval(const double t) const = 0;
};

// Concretions
class SampledTrajectory : public ITrajectory {
public:
  SampledTrajectory(std::vector<std::tuple<double, double, double>> trajectory)
      : trajectory_{trajectory} {};
  std::pair<double, double> eval(const double) const final {
    return std::make_pair<double, double>(12.0,
                                          13.0); // Do the interpolation here
  }
  std::vector<std::tuple<double, double, double>> trajectory_{};
};

class MathModel_One : public ITrajectory {
public:
  std::pair<double, double> eval(const double) const final {
    return std::make_pair<double, double>(
        -1.0,
        -2.0); // Compute according model 1 here
  }
  double coeff_model_one{1.0};
};

class MathModel_Two : public ITrajectory {
public:
  std::pair<double, double> eval(const double) const final {
    return std::make_pair<double, double>(
        -1.0,
        -2.0); // Compute according model 2 here
  }
  std::vector<double> coeff_model_two{{2.0, 3.0}};
};

bool getIntersection(std::pair<double, double> point_A,
                     std::pair<double, double> point_B) {
  return point_A.first == point_B.first;
}

//@class CollisionChecker
//@brief The class itself is not templated. Only one method is temlpated which
//must be speicalized for every compilation
class CollisionChecker {
public:
  CollisionChecker() = default;
  ~CollisionChecker() = default;
  CollisionChecker(CollisionChecker &) = delete;

  bool SamplingCheck(const ITrajectory &traj_A, const ITrajectory &traj_B) {
    std::cout << "Sampling Check (default implementation)";
    // Dont solve analytically using the math model. Instead solve it using
    // sampling approach
    double time = 0.0;
    double time_end = 10.0;
    double time_step = 1.0;
    while (time < time_end) {
      if (getIntersection(traj_A.eval(time), traj_B.eval(time))) {
        return true;
      }
      time += time_step;
    }
    return false;
  }
  template <typename A, typename B>
  bool Check(const A &traj_A, const B &traj_B);
};

template <>
bool CollisionChecker::Check(const SampledTrajectory &traj_A,
                             const MathModel_One &traj_B) {
  std::cout << "Specialized Check between SampledTrajectory and MathModel_One";
  return ((traj_B.coeff_model_one == 0.0) && (traj_A.trajectory_.size() > 10));
}
template <>
bool CollisionChecker::Check(const MathModel_One &traj_A,
                             const SampledTrajectory &traj_B) {
  std::cout << "Solving with MathModel_One and SampledTrajector simply called "
               "the default checker";
  return SamplingCheck(traj_A, traj_B);
}

//@class WarningModule
//@brief Template Class without any specialization. She warns if two
// trajectories are intersecting no matters what type they are
template <typename A, typename B> class WarningModule {
public:
  WarningModule(std::shared_ptr<A> ego, std::shared_ptr<B> obj)
      : ego_traj_{ego}, obj_traj_{obj} {}

  bool WarnIfCollision() {
    CollisionChecker checker{};
    return checker.Check(*ego_traj_, *obj_traj_);
  }

private:
  std::shared_ptr<A> ego_traj_{};
  std::shared_ptr<B> obj_traj_{};
};

int main() {
  std::vector<std::tuple<double, double, double>> points{
      {{0.0, 0.0, 0.0}, {1.0, 0.0, 1.0}}};

  auto ego_traj = std::make_shared<SampledTrajectory>(points);

  auto obj_traj = std::make_shared<MathModel_One>();

  std::shared_ptr<ITrajectory> obj_traj_iface = obj_traj;

  WarningModule<SampledTrajectory, MathModel_One> warn(ego_traj, obj_traj_iface);
  warn.WarnIfCollision();

  auto obj_traj_two = std::make_shared<MathModel_Two>();
  WarningModule<SampledTrajectory, MathModel_Two> warn_two(ego_traj,
                                                           obj_traj_two);
  warn_two.WarnIfCollision();

  return 0;
}
