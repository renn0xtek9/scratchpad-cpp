#ifndef MODULE_H
#define MODULE_H
#include <functional>
#include <iostream>
#include <memory>

#define Signal(Name) struct Name : SignalBase


struct SignalBase {
  std::int64_t timestamp{};
  std::int64_t CRC{};
  bool isValid()=delete;
};

struct PodOne : SignalBase {
  int x{};
  int y{3};
  bool isValid(){return true;}
};

Signal(PodTwo){
  double value1{3.4};
  double value2{-1.0};
};
class Two {
public:
  void triggerTwo(const PodTwo pod_two) {
    std::cout << "Two triggered " << std::endl;
  }
};




class OneA {
public:
  explicit OneA(Two &two_instance,
                std::function<void(Two, const PodTwo)> compute_callback)
      : m_two_insance{two_instance}, m_compute_callback{compute_callback} {}
  void setPod(const PodOne pod) { compute(); }



private:
  void compute() {
    std::cout << "One is computing" << std::endl;
    m_compute_callback(m_two_insance, PodTwo{});
  }
  std::function<void(Two, const PodTwo)> m_compute_callback{};
  Two &m_two_insance;
};

class OneB {
public:
  void trigger() {}
};


// Component (mycomp,OneA,OneB,Two);

struct Component {
  std::shared_ptr<OneA> m_onea;
  std::shared_ptr<Two> m_two;
  std::shared_ptr<OneB> m_oneb;

};



class ComponentFactory {
public:
  explicit ComponentFactory() {}
  
  
  


private:
};

#endif