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
        const auto ans = bigint(0);
        return num == ans;
    }

    static bool test_default_constructor_equal_to_zero_string() {
        const bigint num;
        const auto ans = bigint("0");
        return num == ans;
    }

    static bool test_int64_constructor_equal_to_str_positive() {
        constexpr std::int64_t val = 123;
        const bigint num(val);
        const auto ans = bigint("123");
        return num == ans;
    }

    static bool test_int64_constructor_equal_to_str_negative() {
        constexpr std::int64_t val = -123;
        const bigint num(val);
        const auto ans = bigint("-123");
        return num == ans;
    }

    static bool test_int64_constructor_min() {
        constexpr std::int64_t val = std::numeric_limits<std::int64_t>::min();
        const bigint num(val);
        const auto ans = bigint(std::to_string(val));
        return num == ans;
    }

    void run_all_tests() {
        std::cout <<
                "\nNote: String constructor and == are not explicitly tested as they are always used as part of the other tests as the base."
                "\nFor more detailed reason, please refer to the README.md file.\n"
                "\nRunning tests..."
                << std::endl;

        run_test("Test Default Constructor Equal to Int64 Constructor", test_default_constructor_equal_to_zero_int);
        run_test("Test Default Constructor Equal to String Constructor", test_default_constructor_equal_to_zero_string);
        run_test("Test Int64 Constructor Equal to String Constructor Positive",
                 test_int64_constructor_equal_to_str_positive);
        run_test("Test Int64 Constructor Equal to String Constructor Negative",
                 test_int64_constructor_equal_to_str_negative);
        run_test("Test Int64 Constructor with Min Value", test_int64_constructor_min);


        std::cout << "\nTest Results:\n";
        std::cout << "Passed: " << passed << "/" << total << std::endl;
    }
};

int main() {
    Test test;
    test.run_all_tests();
}
