#pragma once
#include <chrono>
#include <iostream>
#include <string>

namespace al {

class Profiler final {
    std::chrono::steady_clock::time_point m_start;
    std::string m_message;

public:
    explicit Profiler(const std::string &msg = {})
    : m_start(std::chrono::steady_clock::now())
    , m_message(msg) {}

    ~Profiler() {
        using namespace std::chrono;
        auto finish = steady_clock::now();
        auto duration = finish - m_start;

        if (m_message.empty() == false) {
            std::cerr << m_message << ": ";
        }
        std::cerr << duration_cast<milliseconds>(duration).count() << " ms\n";
    }
};

} // namespace al

#define __UNIQ_ID_IMPL__(line_line) _A_LOCAL_VAR_##line_line
#define __UNIQ_ID__(line_line) __UNIQ_ID_IMPL__(line_line)

#define LOG_DURATION(message) al::Profiler __UNIQ_ID__(__LINE__){message}
#define LOG_DURATION_TEXT(text) al::Profiler __UNIQ_ID__(__LINE__){#text}
