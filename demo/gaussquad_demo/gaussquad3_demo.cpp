#include <cmath>
#include <iomanip>
#include <iostream>

#include "allay/gaussquad/tools/quadrature3.hpp"
using quad3 = Quadrature3;  // NOLINT(readability-identifier-naming)

namespace {
// 测试函数1: x^2 + y^2
double func1(double x, double y) { return x * x + y * y; }

// 测试函数2: x * y
double func2(double x, double y) { return x * y; }

// 测试函数3: sin(x) * cos(y)
double func3(double x, double y) { return std::sin(x) * std::cos(y); }
}  // namespace

int main() {
    std::cout << std::setprecision(15);

    quad3::Triangle triangle1{
        .ax = 0.0, .ay = 0.0, .bx = 1.0, .by = 0.0, .cx = 0.0, .cy = 1.0};
    quad3::Triangle triangle2{
        .ax = 0.0, .ay = 0.0, .bx = 2.0, .by = 0.0, .cx = 0.0, .cy = 3.0};

    // (1)
    std::cout << "Using 3 points Gauss Quadrature with func1 (x^2 + y^2): ";
    auto result = quad3{quad3::Builtin::P3}.intg(func1, triangle1);
    std::cout << "\nInt(x^2 + y^2, Triangle) = " << result
              << " (expected 0.166667)\n";

    // (2)
    std::cout << "\nUsing 7 points Gauss Quadrature with func2 (x * y): ";
    result = quad3{}.intg(func2, triangle1);
    std::cout << "\nInt(x * y, Triangle) = " << result
              << " (expected 0.0416667)\n";

    // (3)
    std::cout << "\nUsing 7 points with func3 (sin(x) * "
                 "cos(y)): ";
    result = quad3{quad3::Builtin::P7}.intg(func3, triangle1);
    std::cout << "\nInt(sin(x) * cos(y), Triangle) = " << result
              << " (expected 0.150584)\n";

    // (4) 使用自定义三角形进行积分
    std::cout << "\nUsing custom Triangle (0, 0), (2, 0), (0, 3) with func1 "
                 "(x^2 + y^2): ";
    result = quad3{quad3::Builtin::P12}.intg(func1, triangle2);
    std::cout << "\nInt(x^2 + y^2, Custom Triangle) = " << result
              << " (expected 6.5)\n";

    // (5) 使用不同的测试函数
    std::cout << "\nUsing 12 points with func3 (sin(x) * cos(y)): ";
    result = quad3{quad3::Builtin::P12}.intg(func3, triangle1);
    std::cout << "\nInt(sin(x) * cos(y), Triangle) = " << result
              << " (expected 0.150584)\n";

    return 0;
}
