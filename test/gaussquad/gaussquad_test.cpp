#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "allay/gaussquad/gausslegendre.hpp"
#include "allay/gaussquad/gausslobatto.hpp"

namespace {

bool pass = true;

void TestRuntime(std::pair<std::vector<double>, std::vector<double>> data,
                 unsigned n, unsigned n_min, unsigned n_max) {
    const auto &x = data.first;
    const auto &w = data.second;

    std::cout << "n = " << n << "\n";

    std::cout << "Nodes:   [";
    for (unsigned i = 0; i < x.size(); ++i) {
        std::cout << std::setprecision(15) << x[i];
        if (i != x.size() - 1) std::cout << ";";
    }
    std::cout << "]\n";

    std::cout << "Weights: [";
    for (unsigned i = 0; i < w.size(); ++i) {
        std::cout << std::setprecision(15) << w[i];
        if (i != w.size() - 1) std::cout << ";";
    }
    std::cout << "]\n\n";

    auto f = [](double s, unsigned k) { return std::pow(s, k); };

    for (unsigned k = n_min; k <= n_max + 1; ++k) {
        double exactIntegral = (k % 2 == 0) ? 2.0 / (k + 1) : 0.0;

        double numericalIntegral = 0.0;
        for (size_t i = 0; i < x.size(); ++i) {
            numericalIntegral += f(x[i], k) * w[i];
        }

        double error = std::abs(numericalIntegral - exactIntegral);
        double tol = 10 * std::numeric_limits<double>::epsilon();
        if (k <= n_max && error > tol) {
            std::cerr << "n = " << n << ", k = " << k << ", error = " << error
                      << " " << "tol = " << tol << "\n";
            std::cerr << "Runtime test failed!\n";
            pass = false;
        }
    }
}

template <unsigned N, unsigned Nmin, unsigned Nmax>
void TestCompileTime(
    std::pair<std::array<double, N>, std::array<double, N>> data) {
    const auto &x = data.first;
    const auto &w = data.second;

    std::cout << "n = " << N << "\n";

    std::cout << "Nodes:   [";
    for (unsigned i = 0; i < x.size(); ++i) {
        std::cout << std::setprecision(15) << x.at(i);
        if (i != x.size() - 1) std::cout << ";";
    }
    std::cout << "]\n";

    std::cout << "Weights: [";
    for (unsigned i = 0; i < w.size(); ++i) {
        std::cout << std::setprecision(15) << w.at(i);
        if (i != w.size() - 1) std::cout << ";";
    }
    std::cout << "]\n\n";

    auto f = [](double s, unsigned k) { return std::pow(s, k); };

    for (unsigned k = Nmin; k <= Nmax + 1; ++k) {
        double exactIntegral = (k % 2 == 0) ? 2.0 / (k + 1) : 0.0;

        double numericalIntegral = 0.0;
        for (size_t i = 0; i < x.size(); ++i) {
            numericalIntegral += f(x.at(i), k) * w.at(i);
        }

        double error = std::abs(numericalIntegral - exactIntegral);
        double tol = 10 * std::numeric_limits<double>::epsilon();
        if (k <= Nmax && error > tol) {
            std::cerr << "n = " << N << ", k = " << k << ", error = " << error
                      << " " << "tol = " << tol << "\n";
            std::cerr << "Compile-time test failed!\n";
            pass = false;
        }
    }
}

}  // namespace

int main() {
    std::cout << std::setprecision(15);
    std::streambuf *defaultBuf = std::cout.rdbuf();

    // Gauss-Legendre runtime test
    std::ofstream runtimeOutput1("gauss-legendre-table-runtime.txt",
                                 std::ios::trunc);
    std::cout.rdbuf(runtimeOutput1.rdbuf());

    for (unsigned n = 2; n <= 15; ++n) {
        TestRuntime(gausslegendre(n), n, 1, (2 * n) - 1);
    }

    std::cout.rdbuf(defaultBuf);
    runtimeOutput1.close();

    if (!pass) {
        std::cout << "Gauss-Legendre Runtime test failed!\n";
        return 1;
    }

    // Gauss-Lobatto runtime test
    std::ofstream runtimeOutput2("gauss-lobatto-table-runtime.txt",
                                 std::ios::trunc);
    std::cout.rdbuf(runtimeOutput2.rdbuf());

    for (unsigned n = 2; n <= 15; ++n) {
        TestRuntime(gausslobatto(n), n, 1, (2 * n) - 3);
    }

    std::cout.rdbuf(defaultBuf);
    runtimeOutput2.close();

    if (!pass) {
        std::cout << "Gauss-Lobatto Runtime test failed!\n";
        return 1;
    }

    // Gauss-Legendre compile-time test
    std::ofstream compileOutput1("gauss-legendre-table-compile.txt",
                                 std::ios::trunc);
    std::cout.rdbuf(compileOutput1.rdbuf());

    TestCompileTime<2, 1, (2 * 2) - 1>(gausslegendre<2>());
    TestCompileTime<3, 1, (2 * 3) - 1>(gausslegendre<3>());
    TestCompileTime<4, 1, (2 * 4) - 1>(gausslegendre<4>());
    TestCompileTime<5, 1, (2 * 5) - 1>(gausslegendre<5>());
    TestCompileTime<6, 1, (2 * 6) - 1>(gausslegendre<6>());
    TestCompileTime<7, 1, (2 * 7) - 1>(gausslegendre<7>());
    TestCompileTime<8, 1, (2 * 8) - 1>(gausslegendre<8>());
    TestCompileTime<9, 1, (2 * 9) - 1>(gausslegendre<9>());
    TestCompileTime<10, 1, (2 * 10) - 1>(gausslegendre<10>());
    TestCompileTime<11, 1, (2 * 11) - 1>(gausslegendre<11>());
    TestCompileTime<12, 1, (2 * 12) - 1>(gausslegendre<12>());
    TestCompileTime<13, 1, (2 * 13) - 1>(gausslegendre<13>());
    TestCompileTime<14, 1, (2 * 14) - 1>(gausslegendre<14>());
    TestCompileTime<15, 1, (2 * 15) - 1>(gausslegendre<15>());

    std::cout.rdbuf(defaultBuf);
    compileOutput1.close();

    if (!pass) {
        std::cout << "Gauss-Legendre Compile time test failed!\n";
        return 1;
    }

    // Gauss-Lobatto compile-time test
    std::ofstream compileOutput2("gauss-lobatto-table-compile.txt",
                                 std::ios::trunc);
    std::cout.rdbuf(compileOutput2.rdbuf());

    TestCompileTime<2, 1, (2 * 2) - 3>(gausslobatto<2>());
    TestCompileTime<3, 1, (2 * 3) - 3>(gausslobatto<3>());
    TestCompileTime<4, 1, (2 * 4) - 3>(gausslobatto<4>());
    TestCompileTime<5, 1, (2 * 5) - 3>(gausslobatto<5>());
    TestCompileTime<6, 1, (2 * 6) - 3>(gausslobatto<6>());
    TestCompileTime<7, 1, (2 * 7) - 3>(gausslobatto<7>());
    TestCompileTime<8, 1, (2 * 8) - 3>(gausslobatto<8>());
    TestCompileTime<9, 1, (2 * 9) - 3>(gausslobatto<9>());
    TestCompileTime<10, 1, (2 * 10) - 3>(gausslobatto<10>());
    TestCompileTime<11, 1, (2 * 11) - 3>(gausslobatto<11>());
    TestCompileTime<12, 1, (2 * 12) - 3>(gausslobatto<12>());
    TestCompileTime<13, 1, (2 * 13) - 3>(gausslobatto<13>());
    TestCompileTime<14, 1, (2 * 14) - 3>(gausslobatto<14>());
    TestCompileTime<15, 1, (2 * 15) - 3>(gausslobatto<15>());

    std::cout.rdbuf(defaultBuf);
    compileOutput2.close();

    if (!pass) {
        std::cout << "Gauss-Lobatto Compile time test failed!\n";
        return 1;
    }

    return 0;
}
