# Expression: Symbolic Differentiation Library

## Description
This library provides symbolic differentiation capabilities for mathematical expressions. It allows users to construct expressions, perform algebraic operations, compute derivatives symbolically, and evaluate expressions with real or complex numbers.

## Features
- **Expression class (`Expression<T>`)** representing arithmetic expressions.
- Supports construction from:
  - Numeric values: `Expression(5.8)`
  - Variables: `Expression("x")`
- Supported operations:
  - Arithmetic: `+`, `-`, `*`, `/`, `^` (power)
  - Functions: `sin`, `cos`, `ln` (natural logarithm), `exp` (exponential)
- Convert expressions to strings.
- Substitute variables with values.
- Evaluate expressions with assigned variable values.
- **Template-based:** Supports real (`double`) and complex (`std::complex<double>`) numbers.
- Parse expressions from strings.
- Compute symbolic derivatives with respect to a given variable.
- Comprehensive test coverage with `OK` or `FAIL` verdicts.

## Project Structure
```
📁 expression_project/
├── expression.hpp    # Declaration of the Expression class
├── expression.cpp    # Implementation of the Expression class
├── tests.cpp         # Unit tests for the library
├── Makefile          # Make build script
├── README.md         # Project documentation
```

## Installation & Build
The project is built using `make`:

- **Build the library and all programs:**
  ```sh
  make
  ```

- **Run tests:**
  ```sh
  make test
  ```
  Each test outputs a verdict of `OK` or `FAIL`.

## Requirements
- C++ compiler (GCC or Clang with C++17 support or higher)
- `make`
