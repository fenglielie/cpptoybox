# MParser 命令行参数解析组件

`MParser` 是一个轻量级的 C++ 命令行参数解析组件，支持解析标志（flags）和选项（options），并能够处理回调函数、类型转换以及剩余参数。


## 特点：

1. **标志解析**
   - 支持定义简单的标志，例如 `--help` 或 `--verbose`
   - 可以通过回调函数来定义标志存在时的触发事件
   - 标志是可选的，在解析过程中会对标记出现次数计数

2. **选项解析**
   - 支持带值的选项，例如 `--scale=2.0` 或 `--scale 2.0`
   - 支持的数据类型包括：`int`、`double`、`bool`、`string`、`char`、`size_t`
   - 有两种方式获取选项的值：
     1. 通过回调函数给外部变量赋值，需要自行实现合法性检查
     2. 提供外部变量的引用
   - 选项可以为可选或必选

3. **补充**
   - 标志和选项的名称的合法性要求：主体由字母、数字、下划线组成，区分大小写，可以由 `--` 或 `-` 开头，不会自动生成短选项
   - 标志和选项都需要提供说明字符串
   - 不支持选项的默认值，可以通过说明字符串和外部变量默认值来替代实现


## 使用方法

### 1. 创建 `MParser` 实例
```cpp
auto parser = MParser{};
```

### 2. 添加标志

例如
```cpp
parser.add_flag("--help", "show help message", [&parser]() {
    parser.print_usage();
    exit(0);
});
```

### 3. 添加选项

使用回调函数赋值，可以在回调函数中进行合法性检查
```cpp
double scale = 1.0;
parser.add_option<double>("--scale", "", true, [&scale](double arg) {
    if (arg < 0) { return false; }
    scale = arg;
    return true;
});
```

也可以直接传入外部变量
```cpp
int len = 10;
parser.add_option("--len", "", false, len);
```


### 4. 解析命令行参数

例如
```cpp
if (!parser.parse(argc, argv)) {
    parser.print_usage();
    return 1;
}
```

如果解析失败，输出使用说明并退出。

### 5. 收集多余参数

`get_rest` 会返回未被解析的参数列表，保持原有的顺序。
```cpp
auto rest = parser.get_rest();
for (const auto &s : rest) {
    std::cout << s << '\n';
}
```
