# Practice to synchornize implementation and documentation together

In this example we show how to use CMake code generation feature to centralize parameters and generate documentation and implementation that are synchronized.

- [parameter_list.cmake](./parameter_list.cmake) contains a list of CMake variable that are the parameters required across the project.
- [Requirements.md.in](./Requirements.md.in) is an example of how to define a list of requirements dynamically that will be linked to this list of parameters.
- [main.cpp.in](./main.cpp.in) is an example of how to use the parameter directly inside the implementation.

In this way we use the `parameter_list.cmake` as a single source of truth all accross the code base.


## Drawbacks:
- Using `*.in` file usually result in poor syntax highlighting in IDE's
- The @my_parmeter@ in the cpp mays confuse IDE indexing of the codebase.
- Files who have `*.cpp.in` extenasion may not be catched by the `clang-format` pre-commit hook.
