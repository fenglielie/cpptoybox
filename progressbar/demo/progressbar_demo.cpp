#include "progressbar.hpp"

#include <thread>

int main() {
    ProgressBar demo{1.0};

    for (int i = 1; i < 100; i++) {  // waite a while

        demo.show(i / 100.0);
        std::this_thread::sleep_for(std::chrono::milliseconds(10 + i));

        // std::cout << i << '\n';

        // if (i % 10 == 0) { std::cout << i << '\n'; }

        // if (i == 50) { std::cout << "hello\n"; }
    }

    demo.finished(1);

    return 0;
}
