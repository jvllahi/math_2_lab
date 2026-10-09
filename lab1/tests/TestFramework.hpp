#pragma once

#include <cmath>
#include <exception>
#include <iostream>
#include <string>

inline void reportFailure(int& failed, const char* file, int line, const std::string& message) {
    ++failed;
    std::cerr << file << ':' << line << ": " << message << '\n';
}

#define EXPECT_TRUE(failed, condition)                                                           \
    do {                                                                                         \
        if (!(condition)) {                                                                      \
            reportFailure((failed), __FILE__, __LINE__, "Expectation failed: " #condition);     \
        }                                                                                        \
    } while (false)

#define EXPECT_NEAR(failed, value, expected, tolerance)                                          \
    do {                                                                                         \
        const double _value = static_cast<double>(value);                                        \
        const double _expected = static_cast<double>(expected);                                  \
        const double _tolerance = static_cast<double>(tolerance);                                \
        if (std::fabs(_value - _expected) > _tolerance) {                                        \
            reportFailure((failed), __FILE__, __LINE__,                                          \
                          std::string("Expected ") + #value + " ~= " + #expected);             \
        }                                                                                        \
    } while (false)

#define EXPECT_THROW_INVALID_ARGUMENT(failed, expression)                                        \
    do {                                                                                         \
        bool _thrown = false;                                                                    \
        try {                                                                                    \
            (void)(expression);                                                                  \
        } catch (const std::invalid_argument&) {                                                 \
            _thrown = true;                                                                      \
        } catch (...) {                                                                          \
        }                                                                                        \
        if (!_thrown) {                                                                          \
            reportFailure((failed), __FILE__, __LINE__,                                          \
                          "Expected std::invalid_argument from: " #expression);                 \
        }                                                                                        \
    } while (false)
