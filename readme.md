# eseed math
`namespace esdm`

## Goal
This library aims to provide fast, flexible, and modern vector, matrix, and quaternion functions. `constexpr` is used everywhere possible, including vector and matrix constructors, to maximize compile-time optimizations. As this is intended to be a modern library, and will likely be in development for some time, C++20 has been chosen as the language standard as its *concepts* allow for much cleaner template code. There is no intention to support obscure or old compilers.

This project began as part of a game engine, and is designed primarily with game programming in mind.

## Features

### Scalar functions
[Full commented header](include/eseed/math/ops.hpp)

- Special floating point values
  - Infinity
    - Constant
    - Check
  - NaN
    - Quiet constant
    - Signaling constant
    - Check
  - Constants
    - Pi
  - General functions
    - Absolute value
    - Square
    - Square root
    - Power
  - Rounding
    - Truncate
    - Floor
    - Ceil
    - Round
  - Direct-to-integer rounding
    - Truncate to int
    - Floor to int
    - Ceil to int
    - Round to int

### Vector class
[Full commented header](include/eseed/math/vec.hpp)

- Templated with length and type: `Vec<L, T>`
- Constructors
  - Default
    - `Vec3<float>()` = `[0, 0, 0]`
  - Component-wise
    - `Vec3<float>(x, y)` = `[x, y, 0]`
    - `Vec3<float>(x, y, z)` = `[x, y, z]`
  - Single component
    - `Vec3<float>(n)` = `[n, n, n]`
  - Copy constructor
    - `Vec3<float>(otherFloatVec3 /* [1.f, 2.f, 3.f])` = `[1.f, 2.f, 3.f]`
    - `Vec3<float>(otherIntVec3 /* [1, 2, 3])` = `[1.f, 2.f, 3.f]`
    - `Vec3<float>(otherDoubleVec2 /* [0.5, 1.0] /*)` = `[0.5f, 1.f]`
- Subscript operator
  - `vec[i]`
- Shorthands for 1, 2, 3, and 4-component vectors
  - `Vec1<T>`
    - `x` spatial component alias
    - `r` color component alias
  - `Vec2<T>`
    - `x` and `y` spatial component aliases
    - `r` and `g` color component aliases
  - `Vec3<T>`
    - `x`, `y` and `z` spatial component aliases
    - `r`, `g` and `b` color component aliases
  - `Vec4<T>`
    - `x`, `y`, `z` and `w` spatial component aliases
    - `r`, `g`, `b` and `a` color component aliases
- All operators
  - Comparison
    - `==`
      - `vec == vec`
  - Post and pre increment and decrement
    - `++ --`
      - `vec++`
      - `++vec`
  - Unary
    - `+ - ! ~`
      - `-vec`  
  - Binary
    - `+ - * / % & | ^ << >> && ||`
      - `vec + vec`
      - `vec + number`
      - `number + vec`
  - Assignment
    - `+= -= *= /= %= &= |= ^= <<= >>=`
      - `vec += vec`
      - `vec += number`