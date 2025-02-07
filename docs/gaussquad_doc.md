# gaussquad

A Cpp implementation of [gaussquad](https://github.com/fenglielie/gaussquad) (MATLAB).

Supports both compile-time and runtime computation for Gaussian quadrature rules. (C++20 required)

- compile-time version
    ```cpp
    template <unsigned N>
    consteval auto gausslegendre()
        -> std::pair<std::array<double, N>, std::array<double, N>>;

    template <unsigned N>
    consteval auto gausslobatto()
        -> std::pair<std::array<double, N>, std::array<double, N>>;
    ```

- runtime version
    ```cpp
    inline auto gausslegendre(unsigned n)
        -> std::pair<std::vector<double>, std::vector<double>>;

    inline auto gausslobatto(unsigned n)
        -> std::pair<std::vector<double>, std::vector<double>>;
    ```

example: get points and weights
```cpp
auto [points,weights] = gausslegendre<3>();
auto [points,weights] = gausslobatto<4>();

auto [points,weights] = gausslegendre(5);
auto [points,weights] = gausslobatto(6);
```

example: integrate
```cpp
// (1) default: Gauss-Legendre 5 points
double result1 = Quadrature{}.intg(
    [](double x) { return std::sin(x); }, {.xl = 0, .xr = 2 * atan(1.0)});
std::cout << "\nInt(sin(x),{x,0,pi/2}) = " << result1 << "\n";

// (2)
double result2 = Quadrature{Quadrature::Builtin::Lobatto5}.intg(
    [](double x) { return std::sin(x); }, {.xl = 0, .xr = 2 * atan(1.0)});
std::cout << "\nInt(sin(x),{x,0,pi/2}) = " << result2 << "\n";

// (3)
double result3 = Quadrature{gausslegendre<13>()}.intg(
    [](double x) { return std::sin(x); }, {.xl = 0, .xr = 2 * atan(1.0)});
std::cout << "\nInt(sin(x),{x,0,pi/2}) = " << result3 << "\n";
```

Reference:

- [Legendre-Gauss Quadrature Weights and Nodes](https://ww2.mathworks.cn/matlabcentral/fileexchange/4540-legendre-gauss-quadrature-weights-and-nodes?s_tid=srchtitle_support_results_4_Gauss%20Lobatto)
- [Legende-Gauss-Lobatto nodes and weights](https://ww2.mathworks.cn/matlabcentral/fileexchange/4775-legende-gauss-lobatto-nodes-and-weights?s_tid=srchtitle_support_results_3_Gauss%2520Lobatto)
