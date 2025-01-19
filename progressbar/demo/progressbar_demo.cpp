#include "progressbar.hpp"

#include <thread>

int main() {
    ProgressBar demo(1, false, true);

    for (int i = 0; i < 100; i++) {
        demo.show(i / 100.0);
        // waite a while
        std::this_thread::sleep_for(std::chrono::milliseconds(10 + i));
    }

    demo.finished(1);

    return 0;
}
