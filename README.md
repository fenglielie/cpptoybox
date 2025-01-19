# README

<div style="text-align: center;">
  <img src="https://raw.githubusercontent.com/fenglielie/notes_image/main/img/allay_fly.gif" style="width: 50%" title="Allay"/>
</div>


一些C++练习实现的小工具，不加说明时，默认都是纯头文件。

1. **mtest**：模仿gtest实现的纯头文件版本的测试框架
2. **safe_input**：确保获取安全输入的组件
3. **msignal**：观察者模式的实现，信号传递组件
4. **mlog**：实现的一个简单日志库
5. **data_handler**：实现的一个数据文件读写组件
6. **var_type_dict**：基于模板元编程的异类词典（参考《C++模板元编程实战：一个深度学习框架的初步实现》）
7. **simple_thread_pool**：一个简单的C++线程池（[参考链接](https://www.limerence2017.com/2023/09/17/concpp07/)）
8. **mtracer**：一个简单的函数调用栈追踪组件（使用C++20的`std::source_location`而非传统的`__FILE__`等）
9. **mparser**：一个简单的命令行解析器
10. **ini_parser**：一个简单的 ini 文件解析器
11. **progressbar**：一个简单的命令行进度条显示器（很久之前写的，需要优化完善）


| 编译器 | 版本 | 编译选项 |
| ------ | ------- | -------------- |
| clang  | 18.0.0  | -std=c++20     |
| gcc    | 13.0.0  | -std=c++20     |
| MSVC   | 2022    | /std:c++20     |


编译 (cmake version >= 3.15)
```bash
cmake -S . -B build
cmake --build ./build -j8
```

测试
```bash
cd ./build
ctest -j8
```

运行
```bash
./bin/xxx_demo
```
