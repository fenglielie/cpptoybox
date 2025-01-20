# Allay

<div style="text-align: center;">
  <img src="https://raw.githubusercontent.com/fenglielie/notes_image/main/img/allay_fly.gif" style="width: 30%" title="Allay"/>
</div>

Allay is a collection of small C++ tools for practice, which are header-only libraries unless otherwise specified.



1. **mtest**：A header-only testing framework mimicking gtest.
2. **safe_input**：A component ensuring safe input handling.
3. **msignal**：An observer pattern implementation for signal transmission.
4. **mlog**： A simple logging library.
5. **data_handler**：A component for reading and writing data files.
6. **var_type_dict**：A heterogeneous dictionary based on template metaprogramming (reference: 《C++模板元编程实战：一个深度学习框架的初步实现》).
7. **simple_thread_pool**：A simple C++ thread pool （[reference](https://www.limerence2017.com/2023/09/17/concpp07/)）.
8. **mtracer**：A simple function call stack tracing component (using C++20's `std::source_location` instead of traditional `__FILE__` and others).
9. **mparser**：A simple command-line parser.
10. **ini_parser**：A simple INI file parser.
11. **progressbar**：A simple command-line progress bar display.

---

| compiler | version | flags      |
| -------- | ------- | ---------- |
| clang    | 18.0.0  | -std=c++20 |
| gcc      | 13.0.0  | -std=c++20 |
| MSVC     | 2022    | /std:c++20 |


generate and build (cmake version >= 3.15)
```bash
cmake -S . -B build
cmake --build ./build -j8
```

test
```bash
cd ./build
ctest -j8
```

run
```bash
./bin/xxx_demo
```
