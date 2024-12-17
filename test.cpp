#include "bigint.hpp"
#include <string>
#include <functional>

class Test {
private:
    std::size_t passed = 0;
    std::size_t total = 0;

    // Reference: https://stackoverflow.com/questions/76138430/how-to-pass-function-as-an-argument-to-another-function-in-c
    void run_test(const std::string &name, bool (*test)()) {
        try {
            if (test()) {
                std::cout << "PASSED: " << name << std::endl;
                passed++;
            } else {
                std::cout << "FAILED: " << name << std::endl;
            }
        } catch (const std::exception &e) {
            std::cout << "FAILED: " << name << " - " << e.what() << std::endl;
        }
        total++;
    }

public:
    static bool test_default_constructor_equal_to_zero_int() {
        const bigint num;
        const auto expected = bigint(0);
        return num == expected;
    }

    static bool test_default_constructor_equal_to_zero_string() {
        const bigint num;
        const auto expected = bigint("0");
        return num == expected;
    }

    static bool test_int64_constructor_equal_to_str_positive() {
        constexpr std::int64_t val = 123;
        const bigint num(val);
        const auto expected = bigint("123");
        return num == expected;
    }

    static bool test_int64_constructor_equal_to_str_negative() {
        constexpr std::int64_t val = -123;
        const bigint num(val);
        const auto expected = bigint("-123");
        return num == expected;
    }

    static bool test_int64_constructor_max() {
        constexpr std::int64_t val = std::numeric_limits<std::int64_t>::max();
        const bigint num(val);
        const auto expected = bigint(std::to_string(val));
        return num == expected;
    }

    static bool test_int64_constructor_min() {
        constexpr std::int64_t val = std::numeric_limits<std::int64_t>::min();
        const bigint num(val);
        const auto expected = bigint(std::to_string(val));
        return num == expected;
    }

    static bool test_int64_constructor_negative_zero() {
        const bigint num(-0);
        const auto expected = bigint(0);
        return num == expected;
    }

    static bool test_string_constructor_empty() {
        try {
            bigint num("");
            return false;
        } catch (const std::invalid_argument &) {
            return true;
        }
    }

    static bool test_string_constructor_invalid() {
        try {
            bigint num("-123abc12");
            return false;
        } catch (const std::invalid_argument &) {
            return true;
        }
    }

    static bool test_string_negative_zero() {
        const bigint num("-0");
        const auto expected = bigint(0);
        return num == expected;
    }

    static bool test_string_positive_zero() {
        const bigint num("+0");
        const auto expected = bigint(0);
        return num == expected;
    }

    static bool test_string_long_zero() {
        const bigint num("-000000000");
        const auto expected = bigint(0);
        return num == expected;
    }

    static bool test_string_positive_sign_only() {
        try {
            const bigint num("+");
            return false;
        } catch (const std::invalid_argument &) {
            return true;
        }
    }

    static bool test_string_negative_sign_only() {
        try {
            const bigint num("-");
            return false;
        } catch (const std::invalid_argument &) {
            return true;
        }
    }

    static bool test_negation_positive() {
        const bigint num(123);
        const auto expected = bigint(-123);
        return -num == expected;
    }

    static bool test_negation_negative() {
        const bigint num(-123);
        const auto expected = bigint(123);
        return -num == expected;
    }

    static bool test_negation_zero() {
        const bigint num(0);
        const auto expected = bigint(0);
        return -num == expected;
    }

    static bool test_increment_prefix() {
        bigint num1(123);
        const bigint num2 = ++num1;
        const auto expected = bigint(124);
        return num1 == expected && num2 == expected;
    }

    static bool test_increment_postfix() {
        bigint num1(123);
        const bigint num2 = num1++;
        const auto expected_original = bigint(123);
        const auto expected_incremented = bigint(124);
        return num1 == expected_incremented && num2 == expected_original;
    }

    static bool test_decrement_prefix() {
        bigint num1(123);
        const bigint num2 = --num1;
        const auto expected = bigint(122);
        return num1 == expected && num2 == expected;
    }

    static bool test_decrement_postfix() {
        bigint num1(123);
        const bigint num2 = num1--;
        const auto expected_original = bigint(123);
        const auto expected_decremented = bigint(122);
        return num1 == expected_decremented && num2 == expected_original;
    }

    static bool test_addition_assignment_all_positive() {
        bigint num1(123);
        const bigint num2(456);
        num1 += num2;
        const auto expected = bigint(123 + 456);
        return num1 == expected;
    }

    static bool test_addition_assignment_all_negative() {
        bigint num1(-123);
        const bigint num2(-456);
        num1 += num2;
        const auto expected = bigint(-123 - 456);
        return num1 == expected;
    }

    static bool test_addition_assignment_positive_add_negative() {
        bigint num1(123);
        const bigint num2(-456);
        num1 += num2;
        const auto expected = bigint(123 - 456);
        return num1 == expected;
    }

    static bool test_addition_assignment_negative_add_positive() {
        bigint num1(-123);
        const bigint num2(456);
        num1 += num2;
        const auto expected = bigint(-123 + 456);
        return num1 == expected;
    }

    static bool test_addition_assignment_sum_zero_positive() {
        bigint num1(+123);
        const bigint num2(-123);
        num1 += num2;
        const auto expected = bigint(0);
        return num1 == expected;
    }

    static bool test_addition_assignment_sum_zero_negative() {
        bigint num1(-123);
        const bigint num2(+123);
        num1 += num2;
        const auto expected = bigint(0);
        return num1 == expected;
    }

    void run_all_tests() {
        std::cout <<
                "\nNote: String constructor and == are not explicitly tested as they are always used as part of the other tests as the base."
                "\nFor more detailed reason, please refer to the README.md file.\n"
                "\nRunning tests..."
                << std::endl;

        std::cout << "\nConstructor Tests:\n" << std::endl;
        run_test("Test Default Constructor Equal to Int64 Constructor", test_default_constructor_equal_to_zero_int);
        run_test("Test Default Constructor Equal to String Constructor", test_default_constructor_equal_to_zero_string);
        run_test("Test Int64 Constructor Equal to String Constructor Positive",
                 test_int64_constructor_equal_to_str_positive);
        run_test("Test Int64 Constructor Equal to String Constructor Negative",
                 test_int64_constructor_equal_to_str_negative);
        run_test("Test Int64 Constructor with Max Value", test_int64_constructor_max);
        run_test("Test Int64 Constructor with Min Value", test_int64_constructor_min);
        run_test("Test Int64 Constructor with Negative Zero", test_int64_constructor_negative_zero);
        run_test("Test String Constructor with Invalid String", test_string_constructor_invalid);
        run_test("Test String Constructor with Empty String", test_string_constructor_empty);
        run_test("Test String Constructor with Invalid String", test_string_constructor_invalid);
        run_test("Test String Constructor with Positive Zero", test_string_positive_zero);
        run_test("Test String Constructor with Negative Zero", test_string_negative_zero);
        run_test("Test String Constructor with Long Zero", test_string_long_zero);
        run_test("Test String Constructor with Positive Sign Only", test_string_positive_sign_only);
        run_test("Test String Constructor with Negative Sign Only", test_string_negative_sign_only);

        std::cout << "\nNegation Tests:\n" << std::endl;
        run_test("Test Negation Positive", test_negation_positive);
        run_test("Test Negation Negative", test_negation_negative);
        run_test("Test Negation Zero", test_negation_zero);

        std::cout << "\nIncrement Tests:\n" << std::endl;
        run_test("Test Increment Prefix", test_increment_prefix);
        run_test("Test Increment Postfix", test_increment_postfix);

        std::cout << "\nDecrement Tests:\n" << std::endl;
        run_test("Test Decrement Prefix", test_decrement_prefix);
        run_test("Test Decrement Postfix", test_decrement_postfix);

        std::cout << "Compound Assignment Operators Tests:\n" << std::endl;
        run_test("Test Addition Assignment Positive + Positive", test_addition_assignment_all_positive);
        run_test("Test Addition Assignment Negative + Negative", test_addition_assignment_all_negative);
        run_test("Test Addition Assignment Positive + Negative", test_addition_assignment_positive_add_negative);
        run_test("Test Addition Assignment Negative + Positive", test_addition_assignment_negative_add_positive);
        run_test("Test Addition Assignment Sum Zero with Positive Num", test_addition_assignment_sum_zero_positive);
        run_test("Test Addition Assignment Sum Zero with Negative Num", test_addition_assignment_sum_zero_negative);

        std::cout << "\nTest Results:\n" << std::endl;
        std::cout << "Passed: " << passed << "/" << total << std::endl;
    }
};

int main() {
    Test test;
    test.run_all_tests();
}
