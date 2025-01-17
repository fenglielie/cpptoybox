# README


一些C++练习实现的小工具，不加说明时，默认都是纯头文件。

默认使用 C++20 标准，部分组件在 C++17 标准中也可以正常使用，测试的编译环境如下：

| 编译器 | 版本 | 编译选项 |
| ------ | ------- | -------------- |
| clang  | 18.0.0  | -std=c++20     |
| gcc    | 13.0.0  | -std=c++20     |
| MSVC   | 2022    | /std:c++20     |


- **mtest**：模仿gtest实现的纯头文件版本的测试框架
- **safe_input**：确保获取安全输入的组件
- **msignal**：观察者模式的实现，信号传递组件
- **mlog**：实现的一个简单日志库
- **data_handler**：实现的一个数据文件读写组件
- **var_type_dict**：基于模板元编程的异类词典（参考《C++模板元编程实战：一个深度学习框架的初步实现》）
- **simple_thread_pool**：一个简单的C++线程池（[参考链接](https://www.limerence2017.com/2023/09/17/concpp07/)）
- **mtracer**：一个简单的函数调用栈追踪组件（使用C++20的`std::source_location`而非传统的`__FILE__`等）
- **mparser**：一个简单的命令行解析器
- **ini_parser**：一个极简的 ini 文件解析器

<div style="text-align: center;">
  <img src="https://raw.githubusercontent.com/fenglielie/notes_image/main/img/allay_fly.gif" style="width: 50%" title="Allay"/>
</div>
