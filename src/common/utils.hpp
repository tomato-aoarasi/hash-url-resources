#pragma once

#ifndef __CXX20_UTILS_HPP
#define __CXX20_UTILS_HPP
#include <iostream>
#include <format>
#include <filesystem>

#define __PRINT_FMT(function_name)  template <typename... Tn>inline void function_name(std::format_string<Tn...> fmt, Tn&&... args)

inline namespace self{
    template <typename T>
    [[maybe_unused]] inline void print(const T& arg) {
        std::cout << arg; std::cout.flush();
    }
    template <typename T, typename... Tn>
    [[maybe_unused]] inline void print(const T& arg, const Tn&... args) {
        if constexpr (sizeof...(args) > 0) {
            std::cout << arg << ' ';
        } print(args...);
    }
    template <typename T, typename... Tn>
    [[maybe_unused]] inline void println(const T& arg, const Tn&... args) {
        print(arg, args...); std::cout << std::endl;
    }
    template <typename T>
    [[maybe_unused]] inline void print(T&& arg) {
        std::cout << arg; std::cout.flush();
    }
    template <typename T, typename... Tn>
    [[maybe_unused]] inline void print(T&& arg, Tn&&... args) {
        if constexpr (sizeof...(args) > 0) {
            std::cout << arg << ' ';
        }
        print(args...);
    }
    template <typename T, typename... Tn>
    [[maybe_unused]] inline void println(T&& arg, Tn&&... args) {
        print(arg, args...); std::cout << std::endl;
    }

    __PRINT_FMT(print_fmt) {
        std::cout << std::format(fmt, std::forward<Tn>(args)...);
        std::cout.flush();
    }

    __PRINT_FMT(println_fmt) {
        print_fmt(fmt, std::forward<Tn>(args)...);
        std::cout << std::endl;
    }

    constexpr auto operator""_f(const char* fmt, size_t) {
        return[=]<typename... T>(T&&... Args) { return std::vformat(fmt, std::make_format_args(std::forward<T>(Args)...)); };
    }

    namespace common::utils {
        inline std::string encrypt(const std::string& message) {
            return drogon::utils::getMd5(drogon::utils::getSha256(message));
        }

        inline void createDirector(const std::string& logDirectoryPath) {
            namespace fs = std::filesystem;

            if (!fs::exists(logDirectoryPath)) {
                fs::create_directory(logDirectoryPath);
                LOG_INFO << "Directory created: " << logDirectoryPath;
            }
        }

        std::string str_remove(const std::string& fullString, const std::string& substring) {
            std::size_t found = fullString.find(substring);
            if (found != std::string::npos) {
                return fullString.substr(found + substring.length());
            }
            else {
                throw std::runtime_error("Substring not found in the string.");
            }
        }
    }
}

#undef __PRINT_FMT
#endif // !__CXX20_UTILS_HPP