#include <functional>
#include <iostream>

#ifndef CLASS_H
#define CLASS_H

// This is defined by SEN
using SenCallback = std::function<void()>;

// This is the signature of OnXXChanged method of interface
// we subscribe to and that we want to call
using OnChangedCb = std::function<void(SenCallback&&)>;

// This is a "callback of callback" that will tell the unit
// which method of the SenComponent shall be connected to onXXChanged
// (by calling onXXChanged once the interface is retrieved)
using ConnectorCallback = std::function<void(OnChangedCb)>;

// The actual interface (auto generated)
class DataInterface {
 public:
  void virtual OnChanged(SenCallback&& callback) = 0;
};

// The actual implementation (auto generated)
class DataInterfaceImpl : public DataInterface {
 public:
  void OnChanged(SenCallback&& callback) final {
    std::ignore = callback;
    std::cout << "::OnChanged Implementation" << std::endl;
    callback();
  }
};

// The interface that we mock
class DataInterfaceMock : public DataInterface {
 public:
  void OnChanged(SenCallback&& callback) final {
    std::ignore = callback;
    onchangedCallCounter++;
    std::cout << "::OnChanged Mock" << std::endl;
  }
  std::size_t onchangedCallCounter{};
};

// The unit that we want to test
class MyUnit {
 public:
  MyUnit(ConnectorCallback connectorCb);
  void connect();
  void setInterface(DataInterface* iface);

 private:
  ConnectorCallback connectorCb_;
  DataInterface* iface_;
};

// The component in which the unit is integrated
class MySenComponent {
 public:
  void registered();
  DataInterface* iface_;
  MyUnit* unit_;

  // This is the callback function that shall trigger reupdating the data we want.
  void updateComponentCallback() {
    std::cout << "SenComponent shall be updated!" << std::endl;
  }
};

#endif  // CLASS_H
