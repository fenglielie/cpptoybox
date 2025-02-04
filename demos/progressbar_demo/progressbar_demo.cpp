#include "progressbar/progressbar.hpp"

#include <thread>

int main() {
    ProgressBar demo{2.0};

    for (int i = 1; i < 200; i++) {
        demo.update(i / 100.0);
        std::this_thread::sleep_for(std::chrono::milliseconds(5 + i / 2));

        // if (i % 10 == 0) { std::cout << i << '\n'; }
    }

    demo.update(2.0);

    return 0;
}
