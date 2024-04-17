#include "safe_input.hpp"

int main(int argc, char *argv[]) {
    auto s = SafeInput().next<int>("Please input a int in (0,5): ",
                                   [](int p) { return 0 < p && p < 5; });
    std::cout << "s=" << s << "\n";

    auto d = SafeInput().next<double>("Please input a double: ", true);
    std::cout << "final: d=" << d << "\n";

    auto p = SafeInput().next<bool>("input a bool: ", true);
    std::cout << "final: p=" << p << "\n";

    SafeInput().confirm("Continue or exit(2)?");

    SafeInput().pause();

    return 0;
}
