# bigint: C++ class for arbitrary precision integer calculations

## Introduction

bigint is a C++ class implementation for basic calculations that support arbitrary precision.
The operation supported including `+, +=, ++, -, -=, --, *, *=, -(negation), ==, !=, <, <=, >, >=, <<`.

## Class Parameters (Private)

- `isNegative: bool`: Indicates if a bigint object is positive or negative. Default is false.
- `digits: vector<uint8_t>`: Stores the digits of an arbitrary precision integer. Each element is a digit of the integer
  with range 0 - 9, therefore uint8_t is used to store the digits.

## Class Constructors

- `bigint()`: On default, initializes a bigint object with value 0.
	1. Set `isNegative` to false.
	2. Push 0 to `digits` to represent the value 0.
- `bigint(int64_t)`: Initializes a bigint object with the value of the input integer.
	1. Set `isNegative` to true if the input integer is negative.
	2. Get the absolute value of the input integer.
	3. Push each digit of the absolute value to `digits` in reverse order.
	4. If the input integer is the minimum value of int64_t, call `bigint(string)` constructor to prevent overflow as the absolute value of the minimum value of int64_t is greater than the maximum value of int64_t.
- `bigint(string)`: Initializes a bigint object with the value of the input string.
	1. Check if the input string is empty. If so, throw an `invalid_argument` exception.
    2. Call `str_to_bigint(string)` helper function to store the sign and digits of the input string. The helper function will throw an `invalid_argument` exception if the input string is not a valid integer.

```c++
bigint a; // a = 0
bigint b(123456789); // b = 123456789
bigint c("123456789"); // c = 123456789
```

## Class Methods (Public)

- `- (negation)`: Returns the negation of the bigint object
	1. Change `isNegative` to the opposite if the bigint object is not zero.
	2. Ignore if the bigint object is zero to avoid negative zero and consistency.

- `++ (prefix)`: Increments the bigint object by 1. Returns the current bigint object itself after increment.
	1. Call `+=` to increment the bigint object by 1.
	2. Return the incremented bigint object.

- `++ (postfix)`: Increments the bigint object by 1. Returns the bigint object before increment.
	1. Create a temporary copy of the original bigint object.
	2. Call `+=` to increment the bigint object by 1.
	3. Return the temporary copy of the original bigint object.

- `-- (prefix)`: Decrements the bigint object by 1. Returns the current bigint object itself after decrement.
	1. Call `-=` to decrement the bigint object by 1.
	2. Return the decremented bigint object.

- `-- (postfix)`: Decrements the bigint object by 1. Returns the bigint object before decrement.
	1. Create a temporary copy of the original bigint object.
	2. Call `-=` to decrement the bigint object by 1.
	3. Return the temporary copy of the original bigint object.

- `+=`: Adds a bigint object to the current bigint object. Returns the current bigint object after addition.
  1. Check if two bigint objects have the same sign
  2. If the same, add the two bigint objects by calling `add_abs(vector<uint8_t> &longer, vector<uint8_t> &shorter)` helper function.
  3. If not the same, subtract the two bigint objects by calling `subtract_abs(vector<uint8_t> &big, vector<uint8_t> &small)` helper function.
