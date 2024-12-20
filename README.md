# bigint: C++ class for arbitrary precision integer calculations

## Introduction

bigint is a C++ class implementation for basic calculations that support arbitrary precision.
The operation supported including `+, +=, ++, -, -=, --, *, *=, /, /=, -(negation), ==, !=, <, <=, >, >=, <<`.

Note: For simplicity, in this documentation, `vector`, `string`, `int64_t`, and `uint8_t` refer to `vector`, `string`, `int64_t`, and `uint8_t` from the standard library.

## Class Parameters (Private)

- `isNegative: bool`: Indicates if a bigint object is positive or negative. Default is false.
- `digits: vector<uint8_t>`: Stores the digits of an arbitrary precision integer. Each element is a digit of the integer
  with range 0 - 9, therefore uint8_t is used to store the digits.

## Class Constructors

- `bigint()`: On default, initializes a bigint object with value 0.
	1. Set `isNegative` to false.
	2. Push 0 to `digits` to represent the value 0.

```c++
bigint a;		// a = 0
```

- `bigint(int64_t)`: Initializes a bigint object with the value of the input integer.
	1. Set `isNegative` to true if the input integer is negative.
	2. Get the absolute value of the input integer.
	3. Push each digit of the absolute value to `digits` in reverse order.
	4. If the input integer is the minimum value of int64_t, call `bigint(string)` constructor to prevent overflow as the absolute value of the minimum value of int64_t is greater than the maximum value of int64_t.

```c++
bigint a(123);		// a = 123
bigint b(-123);		// b = -123
```

- `bigint(string)`: Initializes a bigint object with the value of the input string.
	1. Check if the input string is empty. If so, throw an `invalid_argument` exception.
    2. Call `str_to_bigint` helper function to store the sign and digits of the input string. The helper function will throw an `invalid_argument` exception if the input string is not a valid integer.

```c++
bigint c("123");	// c = 123
bigint d("-123");	// d = -123
```

## Class Methods (Private Helpers)
- `void remove_leading_zeros(vector<uint8_t> &digits)`: Removes leading zeros in the input vector.
	1. Iterate through the input vector in reverse order while the last element is zero and the size of the vector is greater than 1.
	2. Pop the last element if it is zero.

- `void str_to_bigint(const string &str)`: Extracts and stores the sign and digits from the input string.
	1. Check if the first char of the input string is a minus sign. If so, set `isNegative` to true.
	2. If the fist char is a sign, check if it is the only char in the string. If so, throw an `invalid_argument` exception. Otherwise, set the start index to 1.
	3. Loop through the input string from the start index to the end. If any char is not a digit, throw an `invalid_argument` exception.
	4. Push each digit to `digits` results the input string is stored in reverse order.
	5. Call `remove_leading_zeros` helper function to remove leading zeros in `digits`.
	6. Call `is_abs_zero` helper function to check if the absolute value of the bigint object is zero. If so, set `isNegative` to false to avoid negative zero.

- `bool is_abs_less_than(const vector<uint8_t> &lhs, const vector<uint8_t> &rhs)`: Returns true if the magnitude of the left-hand side bigint object is less than the right-hand side bigint object, false otherwise.
	1. If the size of `lhs` is less than the size of `rhs`, return true.
	2. Loop through the two vectors in reverse order. If the current digit of `lhs` is less than the digit of `rhs`, return true. If the same, goes into the next iteration. Otherwise, return false.

- `bool is_abs_zero(const vector<uint8_t> &num)`: Returns true if the magnitude of the bigint object is zero, false otherwise.
	1. Check if the size of the input vector is 1 and the only element is zero.

- `vector<uint8_t> add_abs(const vector<uint8_t> &longer, const vector<uint8_t> &shorter)`: Helper function that returns the sum of the magnitudes of two bigint objects.
	1. Create a `result` vector with the size of the `long`. Reserve one more space for the carry.
	2. Initialize a `carry` variable to 0.
	3. Loop through `long`.
      	1. Calculate the sum by adding the current digit of the `long` with the `carry`.
      	2. If the digit of `short` at current index exists, add it to the sum.
      	3. Mod the sum by 10 to get the current digit of the result.
      	4. Divide the sum by 10 to get the carry for next iteration (i.e. 0 or 1).
	4. If there is a `carry` after the loop, push it to the result vector.
	5. Return the `result` vector.

	> **Note:**
	> `add_abs` requires the first parameter to be the vector with a longer size, as the for loop will iterate through the longer vector. Incorrect order will cause incomplete results and unexpected behavior.

- `vector<uint8_t> subtract_abs(const vector<uint8_t> &big, const vector<uint8_t> &small)`: Helper function that returns the difference of the magnitudes of two bigint objects.
	1. Declare a `result` vector and reserve the size of `big`.
	2. Initialize a `borrow` variable to 0.
	3. Loop through `big`.
      	1. Calculate the difference by subtracting the current digit of `big` and `borrow`.
      	2. If the digit of `small` at the current index exists, subtract it from the difference.
      	3. Check if the difference is negative. If so, add 10 to the difference and set `borrow` to 1. Otherwise, set `borrow` to 0.
      	4. Store the current difference to the result vector.
	4. Return the `result` vector.

	> **Note:**
	> `subtract_abs` requires the first parameter to be the vector with a larger magnitude, as the function does not handle negative cases. Incorrect order will cause unexpected behavior.

- `vector<uint8_t> multiply_abs(const vector<uint8_t> &lhs, const vector<uint8_t> &rhs)`: Helper function that returns the product of the magnitudes of two bigint objects.
	1. Check if `lhs` or `rhs` is zero. If so, return a vector with one element 0.
	2. Create a `result` vector of zeros with the size of `lhs` + `rhs`.
	3. Loop through `lhs` with index `i`.
      	1. Initialize a `carry` variable to 0.
	  	2. Loop through `rhs` with index `j`.
			1. Calculate the sum of current digit of `result` at index `i + j` with `carry`.
			2. Add the product of the current digit of `lhs` at index `i` and `rhs` at index `j` to the sum.
			3. Store the sum mod 10 to the current digit of `result` at index `i + j`.
			4. Divide the sum by 10 to get the carry for the next iteration.
    4. If there is a `carry` after the inner loop, set the current digit of `result` at index `i + rhs.size()` to the `carry`.
    5. Return the `result` vector.

	> **Note:**
	> The `multiply_abs` function uses the same algorithm as the manual vertical multiplication algorithm. It uses a nested loop where each digit of one number is multiplied by every digit of the other number. The `result` vector is initialized with zeros instead of just reserving space. This is because, in vertical multiplication, the product of the current digits needs to be added to the digit above it along with `carry`. If the vector is not initialized to zeros, the first iteration will access garbage values. When the inner loop ends, the `carry` is assigned at the index `i + rhs.size()` instead of being pushed to the end of the `result` like `add_abs` and `subtract_abs`. This is because `result` is initialized with zeros, pushing the `carry` to the end will result in an incorrect answer.

- `vector<uint8_t> divide_abs(const vector<uint8_t> &dividend, const vector<uint8_t> &divisor)`: Divides the magnitude of the dividend by the magnitude of the divisor. Returns the quotient as a vector.
	1. Check if the divisor is zero. If so, throw a `logic_error`.
	2. Check if the dividend is zero. If so, return a vector with one element 0.
	3. Create a `quotient` and `current_sum` vector with the size of the dividend with initial value 0.
	4. Loop until `dividend` is less than `current_sum` by calling `is_abs_less_than`
      	1. Call `add_abs` with `current_sum` and `divisor` to get the next sum.
      	2. Add 1 to `quotient`.
	5. Minus 1 from `quotient` correct the value, as the final loop will always cause the `current_sum` to be greater than `dividend` and the `quotient` to be incremented by 1.
	6. Return the `quotient` vector.

	> **Note:**
	> The `divide_abs` function uses the manual long division algorithm. It uses a nested loop where the `divisor` is subtracted from the `temp` vector until it is less than the `divisor`. The `count` is incremented each time the `divisor` is subtracted. The `count` is stored in the `result` vector at the current index. If the `temp` vector is zero, the loop breaks. If the `temp` vector is not zero, the next digit of the `dividend` is pushed to the `temp` vector.

## Class Methods (Public Operators)

- `- (negation)`: Returns the negation of the bigint object
	1. Change `isNegative` to the opposite if the bigint object is not zero.
	2. Ignore if the bigint object is zero to avoid negative zero and consistency.

```c++
bigint a(123);		// a = 123
bigint b = -a;		// b = -123
```

- `++ (prefix)`: Increments the bigint object by 1. Returns the current bigint object itself after increment.
	1. Call `+=` to increment the bigint object by 1.
	2. Return the incremented bigint object.

```c++
bigint a(123);		// a = 123
bigint b = ++a;		// a = 124, b = 124
```

- `++ (postfix)`: Increments the bigint object by 1. Returns the bigint object before increment.
	1. Create a temporary copy of the original bigint object.
	2. Call `+=` to increment the bigint object by 1.
	3. Return the temporary copy of the original bigint object.

```c++
bigint a(123);		// a = 123
bigint b = a++;		// a = 124, b = 123
```

- `-- (prefix)`: Decrements the bigint object by 1. Returns the current bigint object itself after decrement.
	1. Call `-=` to decrement the bigint object by 1.
	2. Return the decremented bigint object.

```c++
bigint a(123);		// a = 123
bigint b = --a;		// a = 122, b = 122
```

- `-- (postfix)`: Decrements the bigint object by 1. Returns the bigint object before decrement.
	1. Create a temporary copy of the original bigint object.
	2. Call `-=` to decrement the bigint object by 1.
	3. Return the temporary copy of the original bigint object.

```c++
bigint a(123);		// a = 123
bigint b = a--;		// a = 122, b = 123
```

- `+=`: Adds a bigint object to the current bigint object. Returns the current bigint object after addition.
  1. Check if two bigint objects have the same sign
  2. If the signs are the same: 
     1. Compare the size of `digits` of the two `bigint` objects. 
     2. Pass the longer one as the first parameter and the shorter one as the second parameter to the `add_abs` helper function to add their magnitudes.
  3. If the signs are NOT the same:
     1. Compare the absolute value of the two `bigint` objects.
     2. Pass the bigger one as the first parameter and the smaller one as the second parameter to the `subtract_abs` helper function to subtract their magnitudes.
  4. Call `remove_leading_zeros` helper function to remove leading zeros in the result.
  5. Check if the result is zero. If so, set `isNegative` to false to avoid negative zero.
  6. Return the current bigint object after addition.

```c++
bigint a(123);		// a = 123
bigint b(456);		// b = 456
a += b;			// a = 579
```

- `-=`: Subtracts a bigint object from the current bigint object. Returns the current bigint object after subtraction.
  1. Call `+=` with the negation of the input bigint object.
  2. Return the current bigint object after subtraction.

```c++
bigint a(123);		// a = 123
bigint b(456);		// b = 456
a -= b;			// a = -333
```

- `*=`: Multiplies a bigint object with the current bigint object. Returns the current bigint object after multiplication.
  1. Check if both bigint objects have the same sign. If not, set `isNegative` to true.
  2. Call `multiply_abs` helper function to multiply the magnitudes of the two bigint objects.
  3. Call `remove_leading_zeros` helper function to remove leading zeros in the result.
  4. Check if the result is zero. If so, set `isNegative` to false to avoid negative zero.

```c++
bigint a(123);		// a = 123
bigint b(456);		// b = 456
a *= b;			// a = 56088
```

- `/=`: Divides the current bigint object by a bigint object. Returns the current bigint object after division.
  1. Check if the divisor bigint object is zero. If so, throw a `logic_error`
  2. Check if both bigint objects have the same sign. If not, set `isNegative` to true.
  3. Call `divide_abs` helper function to divide the magnitudes of the two bigint objects.
  4. Call `remove_leading_zeros` helper function to remove leading zeros in the result.
  5. Check if the result is zero. If so, set `isNegative` to false to avoid negative zero.

```c++
bigint a(456);		// a = 456
bigint b(123);		// b = 123
a /= b;			// a = 3
```

- `+`: Adds two bigint objects. Returns the result of the addition as a new bigint object.
  1. Accepts a copy of the left-hand side bigint object and a reference to the right-hand side bigint object.
  2. Call `+=` with the two input bigint object.
  3. Return the result of the addition.

```c++
bigint a(123);		// a = 123
bigint b(456);		// b = 456
bigint c = a + b;	// c = 579, a = 123, b = 456
```

- `-`: Subtracts two bigint objects. Returns the result of the subtraction as a new bigint object.
  1. Accepts a copy of the left-hand side bigint object and a reference to the right-hand side bigint object.
  2. Call `-=` with the two input bigint object.
  3. Return the result of the subtraction.

```c++
bigint a(123);		// a = 123
bigint b(456);		// b = 456
bigint c = a - b;	// c = -333, a = 123, b = 456
```

- `*`: Multiplies two bigint objects. Returns the result of the multiplication as a new bigint object.
  1. Accepts a copy of the left-hand side bigint object and a reference to the right-hand side bigint object.
  2. Call `*=` with the two input bigint object.
  3. Return the result of the multiplication.

```c++
bigint a(123);		// a = 123
bigint b(456);		// b = 456
bigint c = a * b;	// c = 56088, a = 123, b = 456
```

- `/`: Divides two bigint objects. Returns the result of the division as a new bigint object.
  1. Accepts a copy of the left-hand side bigint object and a reference to the right-hand side bigint object.
  2. Call `/=` with the two input bigint object.
  3. Return the result of the division.

```c++
```

- `==`: Returns true if the two bigint objects are equal, false otherwise.
  1. Check if the signs of the two bigint objects are the same.
  2. Check if the digits of the two bigint objects are the same.

```c++
bigint a(123);		// a = 123
bigint b(123);		// b = 123
bool c = a == b;	// c = true
```

- `!=`: Returns true if the two bigint objects are NOT equal, false otherwise.
  1. Call `==` and return the negation of the result.

```c++
bigint a(123);		// a = 123
bigint b(456);		// b = 456
bool c = a != b;	// c = true
```

- `<`: Returns true if the current bigint object is less than the right-hand side bigint object, false otherwise.
  1. Check if the current bigint object is negative and the right-hand side bigint object is positive. If so, return true.
  2. Check if the current bigint object is positive and the right-hand side bigint object is negative. If so, return false.
  3. If both positive, call `is_abs_less_than` helper function to compare the magnitudes of the two bigint objects.
  4. If both negative, call `!is_abs_less_than` helper function to compare the magnitudes of the two bigint objects. The one with larger magnitude is smaller.

```c++
bigint a(-123);		// a = -123
bigint b(-456);		// b = -456
bool c = a < b;		// c = false
```

- `<=` Returns true if the current bigint object is less than or equal to the right-hand side bigint object, false otherwise.
  1. Call `==` to check if the two bigint objects are equal. If so, return true.
  2. Call `<` and return the result.

```c++
bigint a(123);		// a = 123
bigint b(123);		// b = 123
bool c = a <= b;	// c = true
```

- `>`: Returns true if the current bigint object is greater than the right-hand side bigint object, false otherwise.
  1. Call `<=` and return the negation of the result.

```c++
bigint a(123);		// a = 123
bigint b(456);		// b = 456
bool c = a > b;		// c = false
```

- `>=`: Returns true if the current bigint object is greater than or equal to the right-hand side bigint object, false otherwise.
  1. Call `<` and return the negation of the result.

```c++
bigint a(123);		// a = 123
bigint b(123);		// b = 123
bool c = a >= b;	// c = true
```

- `<<`: Outputs the bigint object to the output stream.
  1. Check if the bigint object is negative. If so, output a minus sign.
  2. Output each digit in `digits` in reverse order.

```c++
bigint a(-123);		// a = 123
cout << a;		// Output: -123
```