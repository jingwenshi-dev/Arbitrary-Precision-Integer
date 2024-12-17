#ifndef BIGINT_HPP
#define BIGINT_HPP

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <ranges>

class bigint {
private:
    bool isNegative{};
    std::vector<std::uint8_t> digits;

    static void remove_leading_zeros(std::vector<std::uint8_t> &digits) {
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
    }

    void str_to_bigint(const std::string &str) {
        isNegative = str[0] == '-';
        size_t start = 0;

        if (str[0] == '-' || str[0] == '+') {
            if (str.size() == 1) throw std::invalid_argument("Error: String contains only a sign char.");
            start = 1;
        }

        for (size_t i = str.length(); i > start; i--) {
            if (!std::isdigit(str[i - 1])) {
                throw std::invalid_argument("Error: String contains non-digit char.");
            }
            // Reference: https://stackoverflow.com/questions/5029840/convert-char-to-int-in-c-and-c
            digits.push_back(static_cast<std::uint8_t>(str[i - 1] - '0'));
        }

        // Handle negative zero
        remove_leading_zeros(this->digits);
        if (is_abs_zero(this->digits)) isNegative = false;
    }

    static bool is_abs_less_than(const std::vector<std::uint8_t> &lhs, const std::vector<std::uint8_t> &rhs) {
        if (lhs.size() != rhs.size()) return lhs.size() < rhs.size();

        for (std::size_t i = lhs.size(); i > 0; --i) {
            if (lhs[i - 1] != rhs[i - 1]) return lhs[i - 1] < rhs[i - 1];
        }

        return false;
    }

    static bool is_abs_zero(const std::vector<std::uint8_t> &num) {
        return num.size() == 1 && num[0] == 0;
    }

    static std::vector<std::uint8_t>
    add_abs(const std::vector<std::uint8_t> &lhs, const std::vector<std::uint8_t> &rhs) {
        std::vector<std::uint8_t> result;
        result.resize(std::max(lhs.size(), rhs.size()));
        result.reserve(result.size() + 1); // Max size, e.g. 999 + 999 = 1998 -> 4 digits

        std::uint8_t carry = 0;

        for (std::size_t i = 0; i < lhs.size(); i++) {
            std::uint8_t sum = carry + lhs[i];
            if (i < rhs.size()) sum += rhs[i];

            result[i] = sum % 10;
            carry = sum / 10;
        }

        if (carry != 0) result.push_back(carry);
        return result;
    }

    static std::vector<std::uint8_t> subtract_abs(const std::vector<std::uint8_t> &big,
                                                  const std::vector<std::uint8_t> &small) {
        std::vector<std::uint8_t> result;
        result.reserve(big.size()); // Min size, e.g. 999 - 0 = 999 -> 3 digits

        std::uint8_t borrow = 0;

        for (std::size_t i = 0; i < big.size(); i++) {
            std::int64_t diff = big[i] - borrow;
            if (i < small.size()) {
                diff -= small[i];
            }

            if (diff < 0) {
                diff += 10;
                // If the digit is negative -> Not enough to subtract -> borrow 1 from next digit -> add 10 to current digit
                borrow = 1; // Borrow 1 from next digit
            } else {
                borrow = 0;
            }
            result.push_back(static_cast<std::uint8_t>(diff));
        }

        return result;
    }

    static std::vector<std::uint8_t> multiply_abs(const std::vector<std::uint8_t> &lhs,
                                                  const std::vector<std::uint8_t> &rhs) {
        if (is_abs_zero(lhs) || is_abs_zero(rhs)) return {0};

        std::vector<std::uint8_t> result;
        result.resize(lhs.size() + rhs.size()); // Max size, e.g. 999 * 999 = 998001 -> 6 digits

        // In vertical multiplication form, it does not matter if the lhs or rhs is above the other, it will result in the same addition pattern.
        for (std::size_t i = 0; i < lhs.size(); i++) {
            std::uint8_t carry = 0;

            for (std::size_t j = 0; j < rhs.size(); j++) {
                std::uint8_t curr = result[i + j] + carry;
                // Add carry and the digit above the current one in vertical form
                curr += static_cast<std::uint8_t>(lhs[i] * rhs[j]);

                result[i + j] = curr % 10;
                carry = static_cast<std::uint8_t>(curr / 10);
            }
            // Handle the last carry
            if (carry) {
                result[i + rhs.size()] = carry;
            }
        }
        return result;
    }

public:
    // Constructors
    explicit bigint() {
        isNegative = false;
        digits.push_back(0);
    }

    explicit bigint(std::int64_t num) {
        isNegative = num < 0;

        // Handle overflow when converting to abs
        // Reference: https://stackoverflow.com/questions/16033201/smallest-values-for-int8-t-and-int64-t
        if (num == std::numeric_limits<std::int64_t>::min()) {
            str_to_bigint(std::to_string(num));
            return;
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

    explicit bigint(const std::string &str) {
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
    bigint &operator++() {
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
    bigint &operator--() {
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
    bigint &operator+=(const bigint &rhs) {
        // Add abs values if signs are the same
        if (this->isNegative == rhs.isNegative) {
            if (this->digits.size() > rhs.digits.size()) {
                this->digits = add_abs(this->digits, rhs.digits);
            } else {
                this->digits = add_abs(rhs.digits, this->digits);
            }
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

        remove_leading_zeros(this->digits);
        if (is_abs_zero(this->digits)) this->isNegative = false;

        return *this;
    }

    bigint &operator-=(const bigint &rhs) {
        *this += -rhs;
        return *this;
    }

    bigint &operator*=(const bigint &rhs) {
        this->isNegative = this->isNegative != rhs.isNegative;
        this->digits = multiply_abs(this->digits, rhs.digits);
        remove_leading_zeros(this->digits);
        if (is_abs_zero(this->digits)) this->isNegative = false;
        return *this;
    }

    // Binary Operators
    // Friend functions to allow using compound assignment operators as it will call private parameters.
    friend bigint operator+(bigint lhs, const bigint &rhs) {
        return lhs += rhs;
    }

    friend bigint operator-(bigint lhs, const bigint &rhs) {
        return lhs -= rhs;
    }

    friend bigint operator*(bigint lhs, const bigint &rhs) {
        return lhs *= rhs;
    }

    // Comparison Operators
    bool operator==(const bigint &rhs) const {
        // Reference: https://learn.microsoft.com/en-us/cpp/cpp/equality-operators-equal-equal-and-exclpt-equal?view=msvc-170
        // In c++, == compares the value of obj, not the address
        return this->isNegative == rhs.isNegative && this->digits == rhs.digits;
    }

    bool operator!=(const bigint &rhs) const {
        return !(*this == rhs);
    }

    bool operator<(const bigint &rhs) const {
        if (this->isNegative && !rhs.isNegative) {
            return true;
        }
        if (!this->isNegative && rhs.isNegative) {
            return false;
        }

        if (this->isNegative) return !is_abs_less_than(this->digits, rhs.digits);
        return is_abs_less_than(this->digits, rhs.digits);
    }

    bool operator<=(const bigint &rhs) const {
        if (*this == rhs) return true;
        return *this < rhs;
    }

    bool operator>(const bigint &rhs) const {
        return !(*this <= rhs);
    }

    bool operator>=(const bigint &rhs) const {
        return !(*this < rhs);
    }

    // Insertion
    // Reference: https://stackoverflow.com/questions/476272/how-can-i-properly-overload-the-operator-for-an-ostream
    friend std::ostream &operator<<(std::ostream &stream, const bigint &num) {
        if (num.isNegative) stream << '-';

        // Reference: https://stackoverflow.com/questions/3610933/iterating-c-vector-from-the-end-to-the-beginning
        for (auto &digit: num.digits | std::views::reverse) stream << static_cast<char>(digit + '0');

        return stream;
    }
};

#endif
