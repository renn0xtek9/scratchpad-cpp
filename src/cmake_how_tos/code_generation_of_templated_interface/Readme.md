# Code generation of templated inteface

## Problem

In this example we have a [templated inteface](./templated_log_interface.h) like this.
```cpp 
template <typename Data> 
class LogInterface{
public: 
    virtual void log(const Data& data)= 0; 
};
``` 

We need to write specialization classes.
* Those are purely derived from `LogInterface`. 
* We want to skip generating all the headers for every types.

*Note* that in this usecases though another easier practive would be to simply template the `log` function of the `LogInterface`


## Approach

- The [LOG_INTERFACE_TYPES](./CMakeLists.txt#L1) contains the various type for which we shall specialize the class.
- The [log_interface_derived.h.in](./log_interface_derived.h.in) is an input file that will be used by cmake to generate actual header file using the [CONFIGURE_FILE](./CMakeLists.txt#L9) functionality. 
- Because generated files are stored in `CMAKE_CURRENT_BINARY_DIR`, we add this directory to the [target_include_directories](./CMakeLists.txt#L16)
- The [log_interface_implementations.cpp](./log_interface_implementations.cpp) contains the implementations of all specialized class.
