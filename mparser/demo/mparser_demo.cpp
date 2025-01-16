#include "mparser.hpp"


int main(int argc, char *argv[]) {
    auto parser = MParser{};

    parser.add_flag("--help", "print help message", [&parser]() {
        parser.print_usage();
        exit(0);
    });

    parser.add_flag("--gzip", "use gzip");

    double scale = 1.0;
    parser.add_option<double>("--scale", "", true, [&scale](double arg) {
        if (arg < 0) { return false; }
        scale = arg;
        return true;
    });

    int len = 10;
    parser.add_option("--len", "default: 10", false, len);

    if (!parser.parse(argc, argv)) {
        parser.print_usage();
        return 0;
    }

    auto rest = parser.get_rest();
    std::cout << "Rest:\n";
    for (const auto &s : rest) { std::cout << s << '\n'; }

    std::cout << "Scale: " << scale << '\n';
    std::cout << "Len: " << len << '\n';
    std::cout << "Use flag: " << parser.get_flag_count("--gzip") << '\n';

    return 0;
}
