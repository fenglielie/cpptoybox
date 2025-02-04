#pragma once

#include <algorithm>
#include <chrono>
#include <cmath>
#include <format>
#include <iostream>
#include <string>

class ProgressBar {
public:
    explicit ProgressBar(double arg_end)
        : m_arg_end(arg_end), m_start_time(std::chrono::steady_clock::now()),
          m_last_time(m_start_time) {
        if (m_arg_end <= 0.0) {
            throw std::invalid_argument(
                "ProgressBar: arg_end must be greater than 0");
        }
    }

    void update(double arg_now) {
        update_data(arg_now);

        std::cout << "\r" << generate_str() << std::flush;
    }

    double time_cost() const {
        auto cost_duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                m_last_time - m_start_time);
        double cost_time = static_cast<double>(cost_duration.count()) / 1000.0;

        return cost_time;  // in seconds
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
        double pct = std::clamp(m_last_arg / m_arg_end, 0.0, 1.0);
        double eta_time = (m_arg_end - m_last_arg) / m_last_rate;

        auto cost_duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                m_last_time - m_start_time);
        double cost_time = static_cast<double>(cost_duration.count()) / 1000.0;

        return std::format(" {}[{:6.2f}%][{}][{:.2f}->{:.2f}] ET:{} RT:{}\033[0m",
                           color_label(pct), pct * 100, pct_bar(pct),
                           m_last_arg, m_arg_end, time_str(cost_time),
                           time_str(eta_time));
    }

    void update_data(double arg_now) {
        if (arg_now < m_last_arg) { return; }

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

    static std::string time_str(double dt) {
        if (dt < 1200) {  // < 20 minutes
            return std::format("{:6.2f}s", dt);
        }
        if (dt < 7200) {  // < 2 hours
            return std::format("{:6.2f}m", dt / 60);
        }
        // > 2 hours
        return std::format("{:6.2f}h", dt / 3600);
    }

    static std::string pct_bar(double pct) {
        int filled_num =
            std::clamp(static_cast<int>(pct * block_num), 0, block_num);
        return std::string(filled_num, fill_char)
               + std::string(block_num - filled_num, ' ');
    }
};
