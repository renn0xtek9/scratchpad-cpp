# Using emplace back with default object
In the [main.cpp](./main.cpp#L15) we are adding object to a vector by using `emplace_back(MyClass{})` with a default object;
This result in the object being unecessary locally created then destroyed.

Instead, using `emplace_back()` will create object directly into the vector. This does not require the move constructor.

The following is the output of main.cpp
```bash
---------------
Wrong: emplace_back(MyClass{})
---------------
Index 0
Construtor
Move constructor
Destructor
Index 1
Construtor
Move constructor
Destructor
Index 2
Construtor
Move constructor
Destructor
---------------
Right: with emplace_back()
---------------
Index 0
Construtor
Index 1
Construtor
Index 2
Construtor
Example: payload without move constructor: END
Destructor
Destructor
Destructor
Destructor
Destructor
Destructor
```