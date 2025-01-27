#pragma once

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <optional>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace mparser_detail {

// MParser选项支持的类型
template <typename T>
concept Supported =
    std::disjunction_v<std::is_same<T, std::string>, std::is_same<T, bool>,
                       std::is_same<T, char>, std::is_same<T, int>,
                       std::is_same<T, double>, std::is_same<T, size_t>>;

template <Supported T>
static constexpr const char *get_type_id() {
    if constexpr (std::is_same_v<T, std::string>) { return "string"; }
    else if constexpr (std::is_same_v<T, bool>) { return "bool"; }
    else if constexpr (std::is_same_v<T, char>) { return "char"; }
    else if constexpr (std::is_same_v<T, int>) { return "int"; }
    else if constexpr (std::is_same_v<T, double>) { return "double"; }
    else if constexpr (std::is_same_v<T, std::size_t>) { return "size_t"; }
    else { return "???"; }
}

// 转换值
template <typename T>
static std::optional<T> case_string_to(std::string value) {
    if constexpr (std::is_same_v<T, std::string>) { return value; }
    else if constexpr (std::is_same_v<T, bool>) {
        std::ranges::transform(value, value.begin(), ::tolower);
        return (value == "true" || value == "yes" || value == "ok"
                || value == "on" || value == "1");
    }
    else {
        T result;
        std::stringstream ss(value);
        ss >> result;
        // bad cast
        if (ss.fail()) { return std::nullopt; }
        return result;
    }
}

}  // namespace mparser_detail

class MParser {
public:
    MParser &add_flag(const std::string &flag, const std::string &desc,
                      std::function<void()> caller) {
        check_valid_name(flag);
        check_unique_name(flag);

        m_flags.insert(
            {flag, FlagInfo{.caller = caller, .desc = desc, .count = 0}});
        return *this;
    }

    // without caller
    MParser &add_flag(const std::string &flag, const std::string &desc) {
        return add_flag(flag, desc, []() {});
    }

    // 添加选项，提供回调setter用于赋值
    template <mparser_detail::Supported T>
    MParser &add_option(const std::string &option, const std::string &desc,
                        bool required, std::function<bool(T)> setter) {
        check_valid_name(option);
        check_unique_name(option);

        // 需要在setter基础上增加一层类型转换，返回值代表：类型转换是否成功，setter是否成功
        auto setter_wrapper = [setter](const std::string &value) {
            auto parsed = mparser_detail::case_string_to<T>(value);
            if (!parsed.has_value()) { return false; }

            return setter(parsed.value());
        };

        m_options.insert(
            {option, OptionInfo{.setter = setter_wrapper,
                                .desc = desc,
                                .required = required,
                                .type_id = mparser_detail::get_type_id<T>()}});

        return *this;
    }

    // 添加选项，提供变量用于赋值
    template <mparser_detail::Supported T>
    MParser &add_option(const std::string &option, const std::string &desc,
                        bool required, T &var) {
        check_valid_name(option);
        check_unique_name(option);

        // 需要在setter基础上增加一层类型转换，返回值代表：类型转换是否成功
        auto setter_wrapper = [&var](const std::string &value) {
            auto parsed = mparser_detail::case_string_to<T>(value);
            if (!parsed.has_value()) { return false; }

            var = parsed.value();
            return true;
        };

        m_options.insert(
            {option, OptionInfo{.setter = setter_wrapper,
                                .desc = desc,
                                .required = required,
                                .type_id = mparser_detail::get_type_id<T>()}});

        return *this;
    }

    // 查询选项是否被设置
    bool is_option_set(const std::string &option) const {
        auto it = m_options.find(option);
        return it != m_options.end() && it->second.setted;
    }

    // 获取标志的计数
    size_t get_flag_count(const std::string &flag) const {
        auto it = m_flags.find(flag);
        return it != m_flags.end() ? it->second.count : 0;
    }

    // 解析参数
    bool parse(int argc, char *argv[]) noexcept {
        try {
            for (int i = 1; i < argc; ++i) {
                std::string arg = argv[i];

                // 检查是否是带 "=" 的选项
                size_t eq_pos = arg.find('=');
                if (eq_pos != std::string::npos) {
                    std::string option = arg.substr(0, eq_pos);  // 提取选项
                    std::string value = arg.substr(eq_pos + 1);  // 提取值

                    if (m_options.contains(option)) {
                        if (m_options[option].setter) {  // 调用选项回调
                            if (m_options[option].setter(value)) {
                                m_options[option].setted = true;
                            }
                            else {  // 转换失败
                                throw std::runtime_error(
                                    (std::ostringstream{}
                                     << "Failed to set option with value: "
                                     << option << "=" << value)
                                        .str());
                            }
                        }
                    }
                    else {
                        throw std::runtime_error("Unknown option with value: "
                                                 + arg);
                    }
                }
                else if (m_options.contains(arg)) {
                    const std::string &option = arg;
                    if (i + 1 >= argc)
                        throw std::runtime_error("Missing value for option: "
                                                 + arg);

                    std::string value = argv[++i];  // 获取选项值

                    if (m_options[option].setter) {  // 调用选项回调
                        if (m_options[option].setter(value)) {
                            m_options[option].setted = true;
                        }
                        else {  // 转换失败
                            throw std::runtime_error(
                                (std::ostringstream{}
                                 << "Failed to set option with value: "
                                 << option << "=" << value)
                                    .str());
                        }
                    }
                }
                else if (m_flags.contains(arg)) {
                    m_flags[arg].count++;  // 计数器自增

                    if (m_flags[arg].caller)
                        m_flags[arg].caller();  // 调用标志回调
                }
                else {  // 收集多余参数
                    m_rest.push_back(arg);
                }
            }

            // 检查必需选项是否提供
            for (const auto &[key, info] : m_options) {
                if (info.required && !info.setted) {
                    throw std::runtime_error("Missing required option: " + key);
                }
            }

            return true;
        }
        catch (const std::exception &e) {
            std::cerr << "Error: " << e.what() << '\n';
            return false;
        }
    }

    // 打印帮助信息
    void print_usage() const {
        std::cout << "Usage:\n";

        std::vector<std::pair<std::string, std::string>> fmt_flag_data;
        std::vector<std::pair<std::string, std::string>> fmt_option_data;
        std::vector<std::pair<std::string, std::string>>
            fmt_option_data_required;

        size_t max_length = 0;

        // 构造格式化字符串并计算最大长度
        for (const auto &[option, info] : m_options) {
            std::string key_with_type = option + "[=" + info.type_id + "]";
            max_length = std::max(max_length, key_with_type.length());

            if (info.required) {
                fmt_option_data_required.emplace_back(key_with_type, info.desc);
            }
            else { fmt_option_data.emplace_back(key_with_type, info.desc); }
        }
        for (const auto &[flag, info] : m_flags) {
            max_length = std::max(max_length, flag.length());
            fmt_flag_data.emplace_back(flag, info.desc);
        }

        max_length += 2;  // 使用 2 个空格对齐

        // 输出选项和标志
        if (!fmt_option_data_required.empty()) {
            std::cout << " - Required options:\n";
        }
        for (const auto &[key, desc] : fmt_option_data_required) {
            std::cout << "    " << std::left
                      << std::setw(static_cast<int>(max_length)) << key << desc
                      << '\n';
        }

        if (!fmt_option_data.empty()) { std::cout << " - Options:\n"; }
        for (const auto &[key, desc] : fmt_option_data) {
            std::cout << "    " << std::left
                      << std::setw(static_cast<int>(max_length)) << key << desc
                      << '\n';
        }

        if (!fmt_option_data.empty()) { std::cout << " - Flags:\n"; }
        for (const auto &[key, desc] : fmt_flag_data) {
            std::cout << "    " << std::left
                      << std::setw(static_cast<int>(max_length)) << key << desc
                      << '\n';
        }
    }

    // 获取多余参数（保持原有顺序）
    const std::vector<std::string> &get_rest() const { return m_rest; }

private:
    struct OptionInfo {
        std::function<bool(const std::string &)> setter;

        const std::string desc;
        const bool required{false};  // 是否必要
        const std::string type_id;   // 类型信息

        bool setted{false};  // 不仅要存在，还要成功被赋值
    };

    struct FlagInfo {
        std::function<void()> caller;

        const std::string desc;

        size_t count{0};  // 计数
    };

    std::unordered_map<std::string, FlagInfo> m_flags;
    std::unordered_map<std::string, OptionInfo> m_options;
    std::vector<std::string> m_rest;

    // 确保名称唯一性
    void check_unique_name(const std::string &name) noexcept {
        if (m_flags.find(name) != m_flags.end()) {
            std::cerr << "MParser error: Flag already exists: " << name;
            exit(1);
        }

        if (m_options.find(name) != m_options.end()) {
            std::cerr << "MParser error: Option already exists: " << name;
            exit(1);
        }
    }

    static void check_valid_name(const std::string &name) noexcept {
        // 正则表达式：零到两个横线 `-`
        // 开头，后接大小写字母、数字或下划线，且总长度不超过 10。
        const std::regex pattern(R"(^-{0,2}[A-Za-z0-9_]{1,8}$)");
        if (!std::regex_match(name, pattern)) {
            std::cerr << "MParser error: Invalid name: " << name;
            exit(1);
        }
    }
};
