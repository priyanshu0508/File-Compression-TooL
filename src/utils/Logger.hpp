#pragma once

#include <fmt/color.h>
#include <fmt/core.h>
#include <string>

namespace utils {

/**
 * @brief Simple utility for colorized console logging using fmt.
 */
class Logger {
public:
    static void info(const std::string& msg) {
        fmt::print(fg(fmt::color::cyan), "[-] {}\n", msg);
    }

    static void success(const std::string& msg) {
        fmt::print(fg(fmt::color::green), "[+] {}\n", msg);
    }

    static void error(const std::string& msg) {
        fmt::print(fg(fmt::color::red) | fmt::emphasis::bold, "[!] Error: {}\n", msg);
    }

    static void header(const std::string& title) {
        fmt::print(fg(fmt::color::yellow) | fmt::emphasis::bold, "\n=== {} ===\n", title);
    }
};

} // namespace utils
