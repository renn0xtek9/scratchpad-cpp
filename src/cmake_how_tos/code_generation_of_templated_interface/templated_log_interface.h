#ifndef TEMPLATED_INTERFACE_H
#define TEMPLATED_INTERFACE_H

template <typename Data>
class LogInterface {
 public:
  virtual void log(const Data& data) = 0;
};

#endif  // TEMPLATED_INTERFACE_H
