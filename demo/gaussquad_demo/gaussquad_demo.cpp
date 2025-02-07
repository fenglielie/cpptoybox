#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>

#include "allay/gaussquad/gausslegendre.hpp"
#include "allay/gaussquad/gausslobatto.hpp"

#include "allay/gaussquad/tools/quadrature.hpp"
using quad = Quadrature;  // NOLINT(readability-identifier-naming)

namespace {
double func(double x) { return x * x; }
}  // namespace

int main() {
    std::cout << std::setprecision(15);

    // (1) Gauss-Legendre 3 points
    std::cout << "Using function with Gauss-Legendre 3 points: ";
    double result = quad{quad::Builtin::Legendre3}.intg(
        std::function<double(double)>(func), {.xl = 0, .xr = 1});
    std::cout << "\nInt(x^2,{x,0,1}) = " << result << "\n";

    // (2) Gauss-Lobatto 5 points
    std::cout << "\nUsing function with Gauss-Lobatto 5 points: ";
    result = quad{quad::Builtin::Lobatto5}.intg(
        std::function<double(double)>(func), {.xl = 0, .xr = 1});
    std::cout << "\nInt(x^2,{x,0,1}) = " << result << "\n";

    std::cout << "\nUsing lambda with Gauss-Legendre 5 points: ";
    auto func1 = [](double x) { return std::sin(x); };
    result = quad{}.intg(func1, {.xl = 0, .xr = 1});
    std::cout << "\nInt(sin(x),{x,0,1}) = " << result << "\n";

    std::cout << "\nChange integration interval: ";
    result = quad{}.intg(std::function<double(double)>(func1),
                         {.xl = 0, .xr = 2 * atan(1.0)});
    std::cout << "\nInt(sin(x),{x,0,pi/2}) = " << result << "\n";

    // get points and weights via gausslegendre<N>();
    std::cout << "\nUsing Gauss-Legendre 13 points: ";
    result =
        quad{gausslegendre<13>()}.intg(func1, {.xl = 0, .xr = 2 * atan(1.0)});
    std::cout << "\nInt(sin(x),{x,0,pi/2}) = " << result << "\n";

    // get points and weights via gausslobatto(n);
    std::cout << "\nUsing Gauss-Lobatto 11 points: ";
    result = quad{gausslobatto(11)}.intg(func1, {.xl = 0, .xr = 2 * atan(1.0)});
    std::cout << "\nInt(sin(x),{x,0,pi/2}) = " << result << "\n";

    return 0;
}
