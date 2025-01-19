#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <ctime>

class ProgressBar {
public:
    ProgressBar(double arg_end, bool disable_show, bool use_color)
        : m_arg_end(arg_end), m_disable_show(disable_show),
          m_use_color(use_color) {
        assert(arg_end > 0 && "arg range illegal");

        m_start_time = clock();
        m_last_time = m_start_time;
    }

    void show(double arg_now, bool is_newline, bool is_end) {
        show_detail(arg_now, is_newline, is_end);
    }

    void show(double arg_now) { show_detail(arg_now, false, false); }

    void finished(double arg_now, bool is_newline) {
        show_detail(arg_now, is_newline, true);
    }

    void finished(double arg_now) { show_detail(arg_now, false, true); }

    double time_cost() const {
        clock_t now_time = clock();
        return static_cast<double>(now_time - m_start_time) / 1000;
    }

private:
    // dynamic char at the end of #
    const std::array<char, 4> m_end_lables{'|', '/', '-', '\\'};
    static constexpr int m_show_block_num_max = 20;
    std::size_t m_lable_index = 0;
    double m_arg_end;  // set by init
    double m_last_arg = 0;

    // information in last Show()
    clock_t m_start_time;
    clock_t m_last_time;

    const bool m_disable_show;
    const bool m_use_color;

    void show_detail(double arg_now, bool is_newline, bool is_end) {
        assert((arg_now >= 0) && (arg_now <= m_arg_end) && "arg range illegal");

        // do nothing if disabled
        if (m_disable_show) return;

        double arg_rate = arg_now * 100.0 / m_arg_end;
        int show_block_num =
            static_cast<int>(0.01 * arg_rate * m_show_block_num_max);

        // newline
        if (!is_newline) { printf("\r"); }
        else { printf("\n"); }

        if (m_use_color) { printf("\x1b[93m[%6.2lf%%][", arg_rate); }
        else { printf("[%6.2lf%%][", arg_rate); }

        // main
        int cnt = 0;
        for (cnt = 0; cnt < show_block_num; cnt++) { printf("#"); }
        if (cnt < m_show_block_num_max) {
            printf("%c", m_end_lables.at(m_lable_index));
            ++cnt;
            m_lable_index = (m_lable_index + 1) % 4;
        }
        while (cnt < m_show_block_num_max) {
            printf(" ");
            ++cnt;
        }

        if (is_end) {  // finished
            printf("][%.2lf->%.2lf] Finished! Cost %.2lfs\n", arg_now,
                   m_arg_end, time_cost());
        }
        else {  // unfinished
            clock_t now_time = clock();
            double next_time = (m_arg_end - arg_now) / (arg_now - m_last_arg)
                               * (now_time - m_last_time) / 1000;
            m_last_time = now_time;
            m_last_arg = arg_now;

            // prediction
            if (next_time < 1200) {  // x < 20 min
                printf("][%.2lf->%.2lf] %7.2lfs", arg_now, m_arg_end,
                       next_time);
            }
            else if (next_time < 7200) {  // 20 min < x < 2h
                printf("][%.2lf->%.2lf] %7.2lfm", arg_now, m_arg_end,
                       next_time / 60);
            }
            else {  // 2h < x
                printf("][%.2lf->%.2lf] %7.2lfh", arg_now, m_arg_end,
                       next_time / 3600);
            }
        }

        if (m_use_color) { printf("\x1b[0m"); }

        fflush(stdout);
        return;
    }
};
