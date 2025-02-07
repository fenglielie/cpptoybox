#include "allay/progressbar/progressbar.hpp"

#include <thread>

int main() {
    ProgressBar demo{2.0};

    for (int i = 1; i <= 200; i++) {
        demo.update(i / 100.0, 20);
        std::this_thread::sleep_for(std::chrono::milliseconds(i / 3));
    }

    ProgressBar demo2{1.0, ProgressBar::Format::PCT_TIME};

    for (int i = 1; i <= 100; i++) {
        demo2.update_newline(i / 100.0, 5);
        std::this_thread::sleep_for(std::chrono::milliseconds(i / 3));

        if (i % 10 == 0) { std::cout << i << '\n'; }
    }

    return 0;
}
