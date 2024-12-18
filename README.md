# bigint: C++ class for arbitrary precision integer calculations

## Introduction

bigint is a C++ class implementation for basic calculations that support arbitrary precision.
The operation supported including `+, +=, ++, -, -=, --, *, *=, -(negation), ==, !=, <, <=, >, >=, <<`.

## Class Constructors

1. `bigint()`: On default, initializes a bigint object with value 0.
2. `bigint(int64_t)`: Initializes a bigint object with the value of the input integer.
3. `bigint(string)`: Initializes a bigint object with the value of the input string.

```c++
bigint a; // a = 0
bigint b(123456789); // b = 123456789
bigint c("123456789"); // c = 123456789
```

## Class Parameters (Private)

1. `isNegative: bool`: Indicates if a bigint object is positive or negative.
2. `digits: vector<uint8_t>`: Stores the digits of an arbitrary precision integer.

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
