# Slicing example

This example illustrates "slicing".
The [BaseClassWithoutVirtualDestructor](./classes.h#L17) has no virtual destructor.
The [DerivedClassWithOwnDestructor](./classes.h#L33) provide its own destructor. 
`DerivedClassWithOwnDestructor` initializes an object from the [derived_ptr](./classes.h#L45).

When an actual `DerivedClassWithOwnDestructor` object is [instantiated and destroyed](./main.cpp#L13) by the RAII. It's custom destructor is called.
However, when an `DerivedClassWithOwnDestructor` object is instantiated over a [BaseClassWithoutVirtualDestructor pointer](./main.cpp#L17), the `DerivedClassWithOwnDestructor` is **sliced**. 
As a result, the RAII will not called the [custom destructor](./classes.h#L39), and the initialize object (on the heap) will not be [deleted](./classes.hL42). This lead to a **memory leak**!
