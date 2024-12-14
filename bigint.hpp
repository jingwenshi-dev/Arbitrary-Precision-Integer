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

    static bool is_abs_less_than(const std::vector<std::uint8_t>& lhs, const std::vector<std::uint8_t>& rhs) {
        if (lhs.size() != rhs.size()) return lhs.size() < rhs.size();

        for (std::vector<std::uint8_t>::size_type i = lhs.size(); i > 0; --i) {
            if (lhs[i-1] != rhs[i-1]) return lhs[i-1] < rhs[i-1];
        }

        return false;
    }

    static std::vector<std::uint8_t> add_abs(const std::vector<std::uint8_t>& lhs, const std::vector<std::uint8_t>& rhs) {

        std::vector<std::uint8_t> result;
        result.reserve(std::max(lhs.size(), rhs.size()) + 1);

        std::uint8_t carry = 0;

        for (std::vector<std::uint8_t>::size_type i = 0; i < lhs.size(); i++) {
            std::uint8_t sum = carry + lhs[i];
            if (i < rhs.size()) sum += rhs[i];

            result[i] = sum % 10;
            carry = sum / 10;
        }

        if (carry != 0) result.push_back(carry);
        return result;
    }

    static std::vector<std::uint8_t> subtract_abs(const std::vector<std::uint8_t>& big, const std::vector<std::uint8_t>& small) {
        std::vector<std::uint8_t> result;

        // TODO: Implement subtraction

        return result;
    }

    static std::vector<std::uint8_t> multiply_abs(const std::vector<std::uint8_t>& lhs, const std::vector<std::uint8_t>& rhs) {
        // bool leftIsZero = this->digits.size() == 1 && this->digits[0] == 0;
        // bool rightIsZero = rhs.size() == 1 && rhs[0] == 0;
        //
        // if (leftIsZero || rightIsZero) {
        //     this->digits = {0};
        //     this->isNegative = false;
        //     return;
        // }

        // TODO: Implement multiplication

        std::vector<std::uint8_t> result;
        return result;
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

    // Operators

    // Negation
    bigint operator-() const {
        // Reference: https://www.geeksforgeeks.org/shallow-copy-and-deep-copy-in-c/
        bigint negative = *this;

        // Ignore negative zero to avoid unexpected behavior
        if (!(negative.digits.size() == 1 && negative.digits[0] == 0)) {
            negative.isNegative = !negative.isNegative;
        }

        return negative;
    }

    // Increment: Prefix
    bigint& operator++() {
        *this += bigint(1);
        return *this;
    }

    // Increment: Postfix
    bigint operator++(int) {
        bigint temp = *this;
        *this += bigint(1);
        return temp;
    }

    // Decrement: Prefix
    bigint& operator--() {
        *this -= bigint(1);
        return *this;
    }

    // Decrement: Postfix
    bigint operator--(int) {
        bigint temp = *this;
        *this -= bigint(1);
        return temp;
    }

    // Compound Assignment Operators
    bigint& operator+=(const bigint& rhs) {
        // Add abs values if signs are the same
        if (this->isNegative == rhs.isNegative) {
            this->digits = add_abs(this->digits, rhs.digits);
        } else {
            if (is_abs_less_than(this->digits, rhs.digits)) {
                // Since two nums are different signs, order of subtraction does not matter but we need to follow the sign of the larger number
                this->digits = subtract_abs(rhs.digits, this->digits);

                // If lhs is small positive, rhs is large negative -> result is negative -> follow rhs sign
                // If lhs is small negative, rhs is large positive -> result is positive -> follow rhs sign
                this->isNegative = rhs.isNegative;
            } else {
                this->digits = subtract_abs(this->digits, rhs.digits);
            }
        }
        return *this;
    }

    bigint& operator-=(const bigint& rhs) {
        *this += -rhs;
        return *this;
    }

    bigint& operator*=(const bigint& rhs) {
        this->isNegative = this->isNegative != rhs.isNegative;
        this->digits = multiply_abs(this->digits, rhs.digits);
        return *this;
    }

};

#endif
