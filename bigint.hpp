#ifndef BIGINT_HPP
#define BIGINT_HPP

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <limits>

class bigint {
private:
    bool isNegative{};
    std::vector<std::uint8_t> digits;

    void str_to_bigint(const std::string& str) {
        isNegative = str[0] == '-';
        size_t start = 0;

        if (str[0] == '-' || str[0] == '+') {
            if (str.size() == 1) throw std::invalid_argument("Invalid number");
            start = 1;
        }

        for (size_t i = start; i < str.length(); i++) {
            if (!std::isdigit(str[i])) {
                throw std::invalid_argument("Invalid number");
            }
            // Reference: https://stackoverflow.com/questions/5029840/convert-char-to-int-in-c-and-c
            digits.push_back(static_cast<std::uint8_t>(str[i] - '0'));
        }
    }

public:
    // Constructors
    bigint() {
        isNegative = false;
        digits.push_back(0);
    }

    explicit bigint(std::int64_t num) {
        isNegative = num < 0;

        // Handle overflow when converting to abs
        // Reference: https://stackoverflow.com/questions/16033201/smallest-values-for-int8-t-and-int64-t
        if (num == std::numeric_limits<std::int64_t>::min()) {
            str_to_bigint(std::to_string(num));
        }

        num = std::abs(num);
        if (num == 0) {
            digits.push_back(0);
        } else {
            while (num != 0) {
                digits.push_back(static_cast<std::uint8_t>(num % 10));
                num /= 10;
            }
        }
    }

    explicit bigint(const std::string& str) {
        if (str.empty()) throw std::invalid_argument("Empty string is not permitted");
        str_to_bigint(str);
    }

};

#endif
