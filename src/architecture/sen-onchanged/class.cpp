#include "class.h"

#include <utility>

void MySenComponent::registered() {
  // This is generated here for the exercise.
  // In reality it is retrieve by subscriber.
  // Subscriber is passed to MyUnit which can retrieve the interface
  iface_ = new DataInterfaceImpl();

  unit_ =
      new MyUnit([this](OnChangedCb&& onChanged) { onChanged(SenCallback{[this]() { updateComponentCallback(); }}); });

  // This is what the subscriber does. This shall happen in the MyUnit logic in reality.
  unit_->setInterface(iface_);

  // This shall happen in MyUnit logic after the interface is retrieved
  unit_->connect();
}

MyUnit::MyUnit(ConnectorCallback connectorCb) : connectorCb_{connectorCb} {
}

void MyUnit::setInterface(DataInterface* iface) {
  iface_ = iface;
}

void MyUnit::connect() {
  OnChangedCb onchanged = [this](SenCallback&& callback) { iface_->OnChanged(std::move(callback)); };
  connectorCb_(onchanged);
}
