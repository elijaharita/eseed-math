# eseed math
`namespace esdm`

## Goal
This library aims to provide fast, flexible, and modern vector, matrix, and quaternion functions. `constexpr` is used everywhere possible, including constructors and operators, to maximize compile-time optimizations. As this is intended to be a modern library, and will likely be under active development for some time, C++20 has been chosen as the language standard as its *concepts* allow for much cleaner template code. There is no intention to support obscure or old compilers.

This project began as part of a game engine, and is designed primarily with game programming in mind.

## Usage

This is a header only library. Simply include the `include` folder in your project. 

Alternatively, link `eseed_math` in CMake:

```
add_subdirectory("path/to/eseed_math/")
target_link_libraries(target eseed_math)
```

## Quick introduction

```cpp
#include <iostream>
#include <eseed/math/vecops.hpp>
#include <eseed/math/matops.hpp>

int main() {
    esdm::Vec3<float> a(1, 2, 3);
    esdm::Vec3<float> b(4, 5, 6);

    std::cout << "a: " << a << std::endl;
    std::cout << "b: " << b << std::endl;

    esdm::Vec4<float> point(5, 0, 0, 1);
    esdm::Mat4<float> rotation = esdm::matrot(esdm::Vec3<float>(0, 1, 0), esdm::pi<float>() * 0.5f);

    std::cout << "point: " << point << std::endl;
    std::cout << "rotation: " << rotation << std::endl;
    
    point *= rotation;

    std::cout << "rotated point: " << point << std::endl;
}
```

## Full feature list

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

- Templated with length and type
  - `esdm::Vec<std::size_t L, typename T>`
- Common-sized aliases
  - `esdm::Vec1<typename T>`
  - `esdm::Vec2<typename T>`
  - `esdm::Vec3<typename T>`
  - `esdm::Vec4<typename T>`
- Constructors
  - Default
    - `esdm::Vec3<float>()` = `[0, 0, 0]`
  - Component-wise
    - `esdm::Vec3<float>(x, y)` = `[x, y, 0]`
    - `esdm::Vec3<float>(x, y, z)` = `[x, y, z]`
  - Copy constructor
    - `esdm::Vec3<float>(otherFloatVec3 /* [1.f, 2.f, 3.f] */)` = `[1.f, 2.f, 3.f]`
    - `esdm::Vec3<float>(otherIntVec3 /* [1, 2, 3] */)` = `[1.f, 2.f, 3.f]`
    - `esdm::Vec3<float>(otherDoubleVec2 /* [0.5, 1.0] */)` = `[0.5f, 1.f]`
- Operators (where applicable)
  - Subscript
    - `[]`
      - `vec[i]`
      - Value and reference
  - Comparison
    - `==`
      - `vec == vec`
  - Component-wise post and pre increment and decrement
    - `++ --`
      - `vec++`
      - `++vec`
  - Component-wise unary
    - `+ - ! ~`
      - `-vec`  
  - Component-wise binary
    - `+ - * / % & | ^ << >> && ||`
      - `vec + vec`
      - `vec + number`
      - `number + vec`
  - Component-wise assignment
    - `+= -= *= /= %= &= |= ^= <<= >>=`
      - `vec += vec`
      - `vec += number`
  - ostream
    - `<<`
      - `std::cout << aVec3 << std::endl;` prints in format "`[x, y, z]`"
- Shorthand component accessors for 1, 2, 3, and 4-component vectors
  - (e.g. `vec[0]` can be replaced with `vec.x` for readability)
  - `Vec1`
    - `x` spatial component alias
    - `r` color component alias
  - `Vec2`
    - `x` and `y` spatial component aliases
    - `r` and `g` color component aliases
  - `Vec3`
    - `x`, `y` and `z` spatial component aliases
    - `r`, `g` and `b` color component aliases
  - `Vec4`
    - `x`, `y`, `z` and `w` spatial component aliases
    - `r`, `g`, `b` and `a` color component aliases

### Vector Functions
[Full commented header](include/eseed/math/vecops.hpp)

- Special floating point values
  - Infinity
    - Check all components
    - Check any component
  - NaN
    - Check all components
    - Check any component
- General component-wise functions
  - Absolute value
  - Square
  - Square root
  - Power
- General vector functions
  - Dot product
  - Cross product
- Component-wise rounding
  - Truncate
  - Floor
  - Ceil
  - Round
- Component-wise direct-to-integer rounding
  - Truncate to int
  - Floor to int
  - Ceil to int
  - Round to int

### Matrix Class
[Full commented header](include/eseed/math/mat.hpp)

- Templated with column size, row size, and type
  - `esdm::Mat<std::size_t M, std::size_t N, typename T>`
- Stored column-major
  - Important! When assigning components to a matrix, make sure the components are in a column-major format
- Shorthands for common sizes
  - Any column and row size up to 4 as `esdm::Mat[M]x[N]<T>`
    - e.g. `esdm::Mat2x4<T>`, `esdm::Mat3x3<T>`
  - Shorter-hand for square matrix as `esdm::Mat[L]<T>`
    - e.g. `esdm::Mat4<T>`
- Constructors
  - Default
    - `esdm::Mat2<float>()` = `[[0, 0], [0, 0]]`
  - Component-wise
    - `esdm::Mat2<float>(1, 2, 3, 4)` = `[[1, 2], [3, 4]]`
- Column and row getters
- Operators
  - Subscript
    - `[]`
      - `mat[i]`
      - Value and reference
      - Returns a column vector
  - Comparison
    - `==`
      - `mat == mat`
  - Component-wise post and pre increment and decrement
    - `++ --`
      - `mat++`
      - `++mat`
  - Component-wise unary
    - `+ - ! ~`
      - `-mat`  
  - Component-wise binary
    - `+ - * / % & | ^ << >> && ||`
      - `mat + mat` (`+ -` only)
      - `mat + number`
      - `number + mat`
  - Component-wise assignment
    - `+= -= *= /= %= &= |= ^= <<= >>=`
      - `mat += mat` (`+= -=` only)
      - `mat += number`
  - Matrix multiplication
    - `*`
      - `mat * mat`
      - `mat * vec`
      - `vec * mat`
  - ostream
    - `<<`
      - `std::cout << aMat2 << std::endl;` prints in format "`[[0, 1], [2, 3]]`"

### Matrix functions
[Full commented header](include/eseed/math/matops.hpp)

- Builders
  - Identity
- General functions
  - Inverse
  - Matrix multiplication
    - Matrix * matrix
    - Matrix * row vector
    - Column vector * matrix
- Matrix generation
  - Translation
  - Rotation