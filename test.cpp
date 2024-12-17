#include "bigint.hpp"
#include <string>
#include <sstream>

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

    static bool test_addition_assignment_zero_add_positive() {
        bigint num1(0);
        const bigint num2(123);
        num1 += num2;
        const auto expected = bigint(123);
        return num1 == expected;
    }

    static bool test_addition_assignment_zero_add_negative() {
        bigint num1(0);
        const bigint num2(-123);
        num1 += num2;
        const auto expected = bigint(-123);
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

    static bool test_addition_assignment_max() {
        constexpr std::int64_t max_int64 = std::numeric_limits<std::int64_t>::max();
        bigint num1(max_int64);
        const bigint num2(max_int64);
        num1 += num2;
        const auto expected = bigint("18446744073709551614");
        return num1 == expected;
    }

    static bool test_addition_assignment_min() {
        constexpr std::int64_t min_int64 = std::numeric_limits<std::int64_t>::min();
        bigint num1(min_int64);
        const bigint num2(min_int64);
        num1 += num2;
        const auto expected = bigint("-18446744073709551616");
        return num1 == expected;
    }

    static bool test_addition_assignment_min_add_max() {
        constexpr std::int64_t max_int64 = std::numeric_limits<std::int64_t>::max();
        constexpr std::int64_t min_int64 = std::numeric_limits<std::int64_t>::min();
        bigint num1(min_int64);
        const bigint num2(max_int64);
        num1 += num2;
        const auto expected = bigint("-1");
        return num1 == expected;
    }

    static bool test_addition_assignment_max_add_min() {
        constexpr std::int64_t max_int64 = std::numeric_limits<std::int64_t>::max();
        constexpr std::int64_t min_int64 = std::numeric_limits<std::int64_t>::min();
        bigint num1(max_int64);
        const bigint num2(min_int64);
        num1 += num2;
        const auto expected = bigint("-1");
        return num1 == expected;
    }

    static bool test_subtraction_assignment_positive_big_minus_small() {
        bigint num1(456);
        const bigint num2(123);
        num1 -= num2;
        const auto expected = bigint(456 - 123);
        return num1 == expected;
    }

    static bool test_subtraction_assignment_positive_small_minus_big() {
        bigint num1(123);
        const bigint num2(456);
        num1 -= num2;
        const auto expected = bigint(123 - 456);
        return num1 == expected;
    }

    static bool test_subtraction_assignment_negative_big_minus_small() {
        bigint num1(-123);
        const bigint num2(-456);
        num1 -= num2;
        const auto expected = bigint(-123 + 456);
        return num1 == expected;
    }

    static bool test_subtraction_assignment_negative_small_minus_big() {
        bigint num1(-456);
        const bigint num2(-123);
        num1 -= num2;
        const auto expected = bigint(-456 + 123);
        return num1 == expected;
    }

    static bool test_subtraction_mixed_big_minus_small() {
        bigint num1(456);
        const bigint num2(-123);
        num1 -= num2;
        const auto expected = bigint(456 + 123);
        return num1 == expected;
    }

    static bool test_subtraction_mixed_small_minus_big() {
        bigint num1(-123);
        const bigint num2(456);
        num1 -= num2;
        const auto expected = bigint(-123 - 456);
        return num1 == expected;
    }

    static bool test_subtraction_diff_zero() {
        bigint num1(123);
        const bigint num2(123);
        num1 -= num2;
        const auto expected = bigint(0);
        return num1 == expected;
    }

    static bool test_subtraction_zero_minus_positive() {
        bigint num1(0);
        const bigint num2(123);
        num1 -= num2;
        const auto expected = bigint(-123);
        return num1 == expected;
    }

    static bool test_subtraction_zero_minus_negative() {
        bigint num1(0);
        const bigint num2(-123);
        num1 -= num2;
        const auto expected = bigint(123);
        return num1 == expected;
    }

    static bool test_subtraction_max_minus_min() {
        constexpr std::int64_t max_int64 = std::numeric_limits<std::int64_t>::max();
        constexpr std::int64_t min_int64 = std::numeric_limits<std::int64_t>::min();
        bigint num1(max_int64);
        const bigint num2(min_int64);
        num1 -= num2;
        const auto expected = bigint("18446744073709551615");
        return num1 == expected;
    }

    static bool test_subtraction_min_minus_max() {
        constexpr std::int64_t max_int64 = std::numeric_limits<std::int64_t>::max();
        constexpr std::int64_t min_int64 = std::numeric_limits<std::int64_t>::min();
        bigint num1(min_int64);
        const bigint num2(max_int64);
        num1 -= num2;
        const auto expected = bigint("-18446744073709551615");
        return num1 == expected;
    }

    static bool test_multiplication_assignment_all_positive() {
        bigint num1(123);
        const bigint num2(456);
        num1 *= num2;
        const auto expected = bigint(123 * 456);
        return num1 == expected;
    }

    static bool test_multiplication_assignment_all_negative() {
        bigint num1(-123);
        const bigint num2(-456);
        num1 *= num2;
        const auto expected = bigint(-123 * -456);
        return num1 == expected;
    }

    static bool test_multiplication_assignment_positive_negative() {
        bigint num1(123);
        const bigint num2(-456);
        num1 *= num2;
        const auto expected = bigint(123 * -456);
        return num1 == expected;
    }

    static bool test_multiplication_assignment_negative_positive() {
        bigint num1(-123);
        const bigint num2(456);
        num1 *= num2;
        const auto expected = bigint(-123 * 456);
        return num1 == expected;
    }

    static bool test_multiplication_assignment_zero_positive() {
        bigint num1(0);
        const bigint num2(123);
        num1 *= num2;
        const auto expected = bigint(0);
        return num1 == expected;
    }

    static bool test_multiplication_assignment_zero_negative() {
        bigint num1(0);
        const bigint num2(-123);
        num1 *= num2;
        const auto expected = bigint(0);
        return num1 == expected;
    }

    static bool test_multiplication_assignment_max() {
        constexpr std::int64_t max_int64 = std::numeric_limits<std::int64_t>::max();
        bigint num1(max_int64);
        const bigint num2(max_int64);
        num1 *= num2;
        const auto expected = bigint("85070591730234615847396907784232501249");
        return num1 == expected;
    }

    static bool test_multiplication_assignment_min() {
        constexpr std::int64_t min_int64 = std::numeric_limits<std::int64_t>::min();
        bigint num1(min_int64);
        const bigint num2(min_int64);
        num1 *= num2;
        const auto expected = bigint("85070591730234615865843651857942052864");
        return num1 == expected;
    }

    static bool test_addition_operator_return() {
        const bigint num1(123);
        const bigint num2(456);
        const auto result = num1 + num2;
        const auto expected = bigint(123 + 456);
        return result == expected;
    }

    static bool test_subtraction_operator_return() {
        const bigint num1(456);
        const bigint num2(123);
        const auto result = num1 - num2;
        const auto expected = bigint(456 - 123);
        return result == expected;
    }

    static bool test_multiplication_operator_return() {
        const bigint num1(123);
        const bigint num2(456);
        const auto result = num1 * num2;
        const auto expected = bigint(123 * 456);
        return result == expected;
    }

    static bool test_inequality_operator() {
        const bigint num1(123);
        const bigint num2(456);
        return num1 != num2;
    }

    static bool test_less_than_operator_all_positive() {
        const bigint num1(123);
        const bigint num2(456);
        return num1 < num2;
    }

    static bool test_less_than_operator_all_negative() {
        const bigint num1(-456);
        const bigint num2(-123);
        return num1 < num2;
    }

    static bool test_less_than_operator_mixed() {
        const bigint num1(-123);
        const bigint num2(123);
        return num1 < num2;
    }

    static bool test_less_than_equal_operator_equal() {
        const bigint num1(123);
        const bigint num2(123);
        return num1 <= num2;
    }

    static bool test_less_than_equal_operator_less_than() {
        const bigint num1(-456);
        const bigint num2(0);
        return num1 <= num2;
    }

    static bool test_greater_than_operator() {
        const bigint num1(123);
        const bigint num2(0);
        return num1 > num2;
    }

    static bool test_greater_than_equal_operator() {
        const bigint num1(123);
        const bigint num2(123);
        return num1 >= num2;
    }

    static bool test_insertion_operator_positive() {
        const bigint num(123);
        std::ostringstream stream;
        stream << num;
        return stream.str() == "123";
    }

    static bool test_insertion_operator_negative() {
        const bigint num(-123);
        std::ostringstream stream;
        stream << num;
        return stream.str() == "-123";
    }

    static bool test_insertion_operator_zero() {
        const bigint num(0);
        std::ostringstream stream;
        stream << num;
        return stream.str() == "0";
    }

    void run_all_tests() {
        std::cout << "\nRunning tests..." << std::endl;

        std::cout << "\nConstructor Tests:" << std::endl;
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

        std::cout << "\nNegation Tests:" << std::endl;
        run_test("Test Negation Positive", test_negation_positive);
        run_test("Test Negation Negative", test_negation_negative);
        run_test("Test Negation Zero", test_negation_zero);

        std::cout << "\nIncrement Tests:" << std::endl;
        run_test("Test Increment Prefix", test_increment_prefix);
        run_test("Test Increment Postfix", test_increment_postfix);

        std::cout << "\nDecrement Tests:" << std::endl;
        run_test("Test Decrement Prefix", test_decrement_prefix);
        run_test("Test Decrement Postfix", test_decrement_postfix);

        std::cout << "\nAddition Assignment Operator Tests:" << std::endl;
        run_test("Test Addition Assignment Positive + Positive", test_addition_assignment_all_positive);
        run_test("Test Addition Assignment Negative + Negative", test_addition_assignment_all_negative);
        run_test("Test Addition Assignment Positive + Negative", test_addition_assignment_positive_add_negative);
        run_test("Test Addition Assignment Negative + Positive", test_addition_assignment_negative_add_positive);
        run_test("Test Addition Assignment Zero + Positive", test_addition_assignment_zero_add_positive);
        run_test("Test Addition Assignment Zero + Negative", test_addition_assignment_zero_add_negative);
        run_test("Test Addition Assignment Sum Zero with Positive + Negative",
                 test_addition_assignment_sum_zero_positive);
        run_test("Test Addition Assignment Sum Zero with Negative + Positive",
                 test_addition_assignment_sum_zero_negative);
        run_test("Test Addition Assignment with Max Int64", test_addition_assignment_max);
        run_test("Test Addition Assignment with Min Int64", test_addition_assignment_min);
        run_test("Test Addition Assignment with Min Int64 + Max Int64", test_addition_assignment_min_add_max);
        run_test("Test Addition Assignment with Max Int64 + Min Int64", test_addition_assignment_max_add_min);

        std::cout << "\nSubtraction Assignment Operator Tests:" << std::endl;
        run_test("Test Subtraction Assignment Positive Big - Small",
                 test_subtraction_assignment_positive_big_minus_small);
        run_test("Test Subtraction Assignment Positive Small - Big",
                 test_subtraction_assignment_positive_small_minus_big);
        run_test("Test Subtraction Assignment Negative Big - Small",
                 test_subtraction_assignment_negative_big_minus_small);
        run_test("Test Subtraction Assignment Negative Small - Big",
                 test_subtraction_assignment_negative_small_minus_big);
        run_test("Test Subtraction Assignment Mixed Big - Small", test_subtraction_mixed_big_minus_small);
        run_test("Test Subtraction Assignment Mixed Small - Big", test_subtraction_mixed_small_minus_big);
        run_test("Test Subtraction Assignment with Difference Zero", test_subtraction_diff_zero);
        run_test("Test Subtraction Assignment with Zero - Positive", test_subtraction_zero_minus_positive);
        run_test("Test Subtraction Assignment with Zero - Negative", test_subtraction_zero_minus_negative);
        run_test("Test Subtraction Assignment with Max Int64 - Min Int64", test_subtraction_max_minus_min);
        run_test("Test Subtraction Assignment with Min Int64 - Max Int 64", test_subtraction_min_minus_max);

        std::cout << "\nMultiplication Assignment Operator Tests:" << std::endl;
        run_test("Test Multiplication Assignment Positive * Positive", test_multiplication_assignment_all_positive);
        run_test("Test Multiplication Assignment Negative * Negative", test_multiplication_assignment_all_negative);
        run_test("Test Multiplication Assignment Positive * Negative",
                 test_multiplication_assignment_positive_negative);
        run_test("Test Multiplication Assignment Negative * Positive",
                 test_multiplication_assignment_negative_positive);
        run_test("Test Multiplication Assignment Zero * Positive", test_multiplication_assignment_zero_positive);
        run_test("Test Multiplication Assignment Zero * Negative", test_multiplication_assignment_zero_negative);
        run_test("Test Multiplication Assignment with Max Int64", test_multiplication_assignment_max);
        run_test("Test Multiplication Assignment with Min Int64", test_multiplication_assignment_min);

        std::cout << "\nBinary Operator Tests:" << std::endl;
        std::cout <<
                "Note: Binary operators call the compound assignment operators, so they only have one test case each to check if values are returned correctly."
                << std::endl;
        run_test("Test Addition Operator Returns", test_addition_operator_return);
        run_test("Test Subtraction Operator Returns", test_subtraction_operator_return);
        run_test("Test Multiplication Operator Returns", test_multiplication_operator_return);

        std::cout << "\nComparison Operator Tests:" << std::endl;
        std::cout <<
                "Note: != depends on ==, > depends on <=, >= depends on <, so them only have 1 test case each to check if values are returned correctly."
                "<= depends on < and ==, so it is tested separately for both cases."
                "< is tested for all positive, all negative, and mixed cases as it is the base of all operators."
                "== is not tested as it is the fundamental of all operators and is used in all tests."
                << std::endl;
        run_test("Test Inequality Operator", test_inequality_operator);
        run_test("Test Less Than Operator All Positive", test_less_than_operator_all_positive);
        run_test("Test Less Than Operator All Negative", test_less_than_operator_all_negative);
        run_test("Test Less Than Operator Mixed", test_less_than_operator_mixed);
        run_test("Test Less Than Equal Operator Equal", test_less_than_equal_operator_equal);
        run_test("Test Less Than Equal Operator Less Than", test_less_than_equal_operator_less_than);
        run_test("Test Greater Than Operator", test_greater_than_operator);
        run_test("Test Greater Than Equal Operator", test_greater_than_equal_operator);

        std::cout << "Insertion Operator Tests:" << std::endl;
        run_test("Test Insertion Operator Positive", test_insertion_operator_positive);
        run_test("Test Insertion Operator Negative", test_insertion_operator_negative);
        run_test("Test Insertion Operator Zero", test_insertion_operator_zero);

        std::cout << "\nTest Results:" << std::endl;
        std::cout << "Passed: " << passed << std::endl;
        std::cout << "Failed: " << total - passed << std::endl;
        std::cout << "Total: " << passed << "/" << total << std::endl;
    }
};

int main() {
    Test test;
    test.run_all_tests();
}
