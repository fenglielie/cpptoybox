#pragma once

#include <chrono>
#include <cmath>
#include <format>
#include <iostream>
#include <string>

class ProgressBar {
public:
    explicit ProgressBar(double arg_end)
        : m_arg_end(arg_end), m_start_time(std::chrono::steady_clock::now()),
          m_last_time(m_start_time) {}

    void update(double arg_now) {
        data_update(arg_now);

        std::cout << "\r" << generate_str() << std::flush;
    }

    void finished() {
        update(m_arg_end);

        std::cout << std::format(" Cost {:.2f}s\n", time_cost());
    }

    double time_cost() const {
        auto time_now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            time_now - m_start_time);
        return static_cast<double>(duration.count()) / 1000.0;  // in seconds
    }

private:
    static constexpr int block_num = 20;
    static constexpr char fill_char = '#';
    static constexpr double alpha = 0.4;  // Smoothing factor

    const double m_arg_end;
    const std::chrono::steady_clock::time_point m_start_time;

    std::chrono::steady_clock::time_point m_last_time;
    double m_last_arg = 0;
    double m_last_rate = 0.0;

    bool m_initialized = false;

    std::string generate_str() const {
        double pct = (m_last_arg / m_arg_end);
        double eta_time = (m_arg_end - m_last_arg) / m_last_rate;

        return std::format(" {}[{:6.2f}%][{}][{:.2f}->{:.2f}]{} \033[0m",
                           color_label(pct), pct * 100, pct_bar(pct),
                           m_last_arg, m_arg_end, time_str(eta_time));
    }

    void data_update(double arg_now) {
        auto time_now = std::chrono::steady_clock::now();

        // Time prediction
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            time_now - m_last_time);
        double elapsed_time = static_cast<double>(duration.count()) / 1000.0;

        // Progress rate
        double cur_rate = (arg_now - m_last_arg) / elapsed_time;

        // If m_last_rate is illegal, use cur_rate only.
        bool illegal_flag = std::isinf(m_last_rate) || std::isnan(m_last_rate);

        if (!m_initialized || illegal_flag) {
            m_last_rate = cur_rate;
            m_initialized = true;
        }
        else {  // Apply exponential smoothing
            m_last_rate = alpha * cur_rate + (1 - alpha) * m_last_rate;
        }

        // Update
        m_last_time = time_now;
        m_last_arg = arg_now;
    }

    static std::string color_label(double pct) {
        int r = 0;
        int g = 0;
        int b = 0;

        if (pct < 0.5) {
            // From yellow to green (0% to 50%)
            r = 255;
            g = static_cast<int>(255 * (pct / 0.5));
            b = 0;
        }
        else {
            // From green to blue (50% to 100%)
            r = static_cast<int>(255 * (1 - (pct - 0.5) / 0.5));
            g = 255;
            b = static_cast<int>(255 * ((pct - 0.5) / 0.5));
        }

        // Return the ANSI escape code for 24-bit RGB color
        return std::format("\033[38;2;{};{};{}m", r, g, b);
    }

    static std::string time_str(double predicted_time) {
        if (predicted_time < 1200) {  // less than 20 minutes
            return std::format("{:6.2f}s", predicted_time);
        }
        if (predicted_time < 7200) {  // less than 2 hours
            return std::format("{:6.2f}m", predicted_time / 60);
        }
        // more than 2 hours
        return std::format("{:6.2f}h", predicted_time / 3600);
    }

    static std::string pct_bar(double pct) {
        int filled_num = static_cast<int>(pct * block_num);

        // Construct progress bar
        std::string pct_bar;
        pct_bar.reserve(block_num);
        pct_bar.append(filled_num, fill_char);

        if (filled_num < block_num) {
            int last_char = static_cast<int>(pct * 100) % 10;
            pct_bar.push_back(std::to_string(last_char)[0]);
            pct_bar.append(block_num - filled_num - 1, ' ');
        }

        return pct_bar;
    }
};
