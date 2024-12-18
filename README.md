# bigint: C++ class for arbitrary precision integer calculations

## Introduction

bigint is a C++ class implementation for basic calculations that support arbitrary precision.
The operation supported including `+, +=, ++, -, -=, --, *, *=, -(negation), ==, !=, <, <=, >, >=, <<`.

Note: For simplicity, in this documentation, `vector`, `string`, `int64_t`, and `uint8_t` refer to `std::vector`, `std::string`, `std::int64_t`, and `std::uint8_t` from the standard library.

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

## Class Methods (Public)

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
std::cout << a;		// Output: -123
```