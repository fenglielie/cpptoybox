#pragma once

#if defined(_MSC_VER)
#pragma warning(disable : 4996)

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#endif

#ifdef _WIN32

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>

#include <optional>
#include <string>

class WindowsConsole {
public:
    static int init() noexcept {
        try {
            SetConsoleOutputCP(65001);
            SetConsoleCP(65001);

            // Set output mode to handle virtual terminal sequences
            // To support ansi colorful output
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            if (hOut == INVALID_HANDLE_VALUE) { return -1; }

            DWORD dwMode = 0;
            if (GetConsoleMode(hOut, &dwMode) == 0) { return -1; }

            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
        catch (...) {
            return -1;
        }
        return 0;
    }

    // Use Windows API to read wide characters from the console and convert to
    // UTF-8.
    static std::optional<std::string> utf8_input(int wide_buf_size) {
        if (wide_buf_size < 64) { wide_buf_size = 64; }
        int utf8_buf_size = wide_buf_size * 4;

        std::wstring wide_buffer(wide_buf_size, L'\0');
        DWORD read_len = 0;
        if (ReadConsoleW(GetStdHandle(STD_INPUT_HANDLE), wide_buffer.data(),
                         wide_buf_size, &read_len, nullptr)
            == 0) {  // Reading failed
            printf("fail 1\n");
            return std::nullopt;
        }

        std::string utf8_buffer(utf8_buf_size, '\0');
        BOOL use_default_char = 0;

        // Convert wide char to UTF-8
        int len = WideCharToMultiByte(
            CP_UTF8, 0, wide_buffer.c_str(), static_cast<int>(read_len),
            utf8_buffer.data(), utf8_buf_size, nullptr, &use_default_char);
        if (use_default_char != 0 || len == 0) {  // Conversion failed
            return std::nullopt;
        }

        if (len < 2 || utf8_buffer[len - 2] != '\r'
            || utf8_buffer[len - 1] != '\n') {
            // Invalid result, missing CRLF at the end
            return std::nullopt;
        }

        return utf8_buffer.substr(0, len - 2);  // Remove CRLF
    }
};
#endif
