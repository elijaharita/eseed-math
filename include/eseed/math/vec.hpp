// Copyright (c) 2020 Elijah Seed Arita
//
// Permission is hereby granted, free of charge, to any person obtaining a copy 
// of this software and associated documentation files (the "Software"), to deal 
// in the Software without restriction, including without limitation the rights 
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
// copies of the Software, and to permit persons to whom the Software is 
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in 
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
// SOFTWARE.

#pragma once

#include "ops.hpp"

#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <ostream>
#include <array>
#include <algorithm>
#include <concepts>

namespace esdm {

template <std::size_t L, typename T>
class VecData {
public:
    T data[L];
};

template <typename T>
class VecData<1, T> {
public:
    union {
        T data[1];
        T x;
        T r;
    };
};

template <typename T>
class VecData<2, T> {
public:
    union {
        T data[2];
        struct { T x, y; };
        struct { T r, g; };
    };
};

template <typename T>
class VecData<3, T> {
public:
    union {
        T data[3];
        struct { T x, y, z; };
        struct { T r, g, b; };
    };
};

template <typename T>
class VecData<4, T> {
public:
    union {
        T data[4];
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };
    };
};

// Forward declaration for shorthand aliases
template <std::size_t L, typename T>
class Vec;

// Shorthand aliases

template <typename T>
using Vec1 = Vec<1, T>;

template <typename T>
using Vec2 = Vec<2, T>;

template <typename T>
using Vec3 = Vec<3, T>;

template <typename T>
using Vec4 = Vec<4, T>;

template <std::size_t L, typename T>
class Vec : public VecData<L, T> {
public:
    // Vec<3, T>(): [ 0, 0, 0 ]
    constexpr Vec() : VecData<L, T>{0} {}

    // Multi element
    // Vec<3, T>(x, y, z) => [ x, y, z ]
    // Vec<3, T>(x, y) => [ x, y, 0 ]
    template <ConvertibleTo<T>... Ts> requires (sizeof...(Ts) <= L)
    constexpr Vec(const Ts&... components) : VecData<L, T>{((T)components)...} {}

    // Type / length conversion (explicit)
    // If length is smaller, trailing elements are cut
    // If length is larger, additional elements are default initialized
    template <ConvertibleTo<T> T1, std::size_t L1>
    constexpr explicit Vec(const Vec<L1, T1>& other) {
        for (std::size_t i = 0; i < std::min(L, L1); i++) data[i] = (T)other[i];
    }

    // Type conversion only (implicit)
    template <ConvertibleTo<T> T1>
    constexpr Vec(const Vec<L, T1>& other) {
        for (std::size_t i = 0; i < L; i++) data[i] = (T)other[i];
    }

    constexpr T operator[](std::size_t i) const {
        return data[i];
    }

    constexpr T& operator[](std::size_t i) {
        return data[i];
    }

    constexpr std::string toString() const {
        std::string out = "[";
        for (std::size_t i = 0; i < L; i++) {
            out += std::to_string(data[i]);
            if (i < L - 1) out += ", ";
        }
        out += "]";
        return out;
    }

    friend std::ostream& operator<<(std::ostream& out, const Vec& v) {
        out << v.toString();
        return out;
    }
};

// -- OPERATORS -- //

template <std::size_t L, typename T0, typename T1, typename = decltype(T0(0) == T1(0))> 
constexpr bool operator==(const Vec<L, T0>& a, const Vec<L, T1>& b) {              
    for (std::size_t i = 0; i < L; i++) if (a[i] != b[i]) return false;                 
    return true;                                                                   
}                                                                                  

// Pre-increment and decrement
#define ESEED_VEC_PRE(op)                                                      \
    template <std::size_t L, typename T, typename = decltype((*(new T(0)))op)> \
    constexpr Vec<L, T>& operator op(Vec<L, T>& v) {                           \
        for (std::size_t i = 0; i < L; i++) op v[i];                           \
        return v;                                                              \
    }
ESEED_VEC_PRE(++)
ESEED_VEC_PRE(--)
#undef ESEED_VEC_PRE

// Post-increment and decrement
#define ESEED_VEC_POST(op)                                                     \
    template <std::size_t L, typename T, typename = decltype((*(new T(0)))op)> \
    constexpr Vec<L, T> operator op(Vec<L, T>& v, int) {                       \
        Vec<L, T> out = v;                                                     \
        for (std::size_t i = 0; i < L; i++) v[i] op;                           \
        return out;                                                            \
    }
ESEED_VEC_POST(--)
ESEED_VEC_POST(++)
#undef ESEED_VEC_POST

// Unary
#define ESEED_VEC_UN(op)                                               \
    template <std::size_t L, typename T, typename = decltype(op T(0))> \
    constexpr Vec<L, T> operator op(const Vec<L, T>& v) {              \
        Vec<L, T> out;                                                 \
        for (std::size_t i = 0; i < L; i++) out[i] = op v[i];          \
        return out;                                                    \
    }
ESEED_VEC_UN(+)
ESEED_VEC_UN(-)
ESEED_VEC_UN(!)
ESEED_VEC_UN(~)
#undef ESEED_VEC_UN

// Binary vector-vector
#define ESEED_VEC_BIN_VV(op)                                                                     \
    template <std::size_t L, typename T0, typename T1, typename TRes = decltype(T0(0) op T1(0))> \
    constexpr Vec<L, TRes> operator op(const Vec<L, T0>& a, const Vec<L, T1>& b) {               \
        Vec<L, TRes> out;                                                                        \
        for (std::size_t i = 0; i < L; i++) out[i] = a[i] op b[i];                               \
        return out;                                                                              \
    }
ESEED_VEC_BIN_VV(+)
ESEED_VEC_BIN_VV(-)
ESEED_VEC_BIN_VV(*)
ESEED_VEC_BIN_VV(/)
ESEED_VEC_BIN_VV(%)
ESEED_VEC_BIN_VV(&)
ESEED_VEC_BIN_VV(|)
ESEED_VEC_BIN_VV(^)
ESEED_VEC_BIN_VV(<<)
ESEED_VEC_BIN_VV(>>)
ESEED_VEC_BIN_VV(&&)
ESEED_VEC_BIN_VV(||)
#undef ESEED_VEC_BIN_VV

// Binary vector-scalar
#define ESEED_VEC_BIN_VS(op)                                                                     \
    template <std::size_t L, typename T0, typename T1, typename TRes = decltype(T0(0) op T1(0))> \
    constexpr Vec<L, TRes> operator op(const Vec<L, T0>& a, T1 b) {                              \
        Vec<L, TRes> out;                                                                        \
        for (std::size_t i = 0; i < L; i++) out[i] = a[i] op b;                                  \
        return out;                                                                              \
    }
ESEED_VEC_BIN_VS(+)
ESEED_VEC_BIN_VS(-)
ESEED_VEC_BIN_VS(*)
ESEED_VEC_BIN_VS(/)
ESEED_VEC_BIN_VS(%)
ESEED_VEC_BIN_VS(&)
ESEED_VEC_BIN_VS(|)
ESEED_VEC_BIN_VS(^)
ESEED_VEC_BIN_VS(<<)
ESEED_VEC_BIN_VS(>>)
ESEED_VEC_BIN_VS(&&)
ESEED_VEC_BIN_VS(||)
#undef ESEED_VEC_BIN_VS

// Binary scalar-vector
#define ESEED_VEC_BIN_SV(op)                                                                     \
    template <std::size_t L, typename T0, typename T1, typename TRes = decltype(T0(0) op T1(0))> \
    constexpr Vec<L, TRes> operator op(const T0& a, const Vec<L, T1>& b) {                       \
        Vec<L, TRes> out;                                                                        \
        for (std::size_t i = 0; i < L; i++) out[i] = a op b[i];                                  \
        return out;                                                                              \
    }
ESEED_VEC_BIN_SV(+)
ESEED_VEC_BIN_SV(-)
ESEED_VEC_BIN_SV(*)
ESEED_VEC_BIN_SV(/)
ESEED_VEC_BIN_SV(%)
ESEED_VEC_BIN_SV(&)
ESEED_VEC_BIN_SV(|)
ESEED_VEC_BIN_SV(^)
ESEED_VEC_BIN_SV(<<)
ESEED_VEC_BIN_SV(>>)
ESEED_VEC_BIN_SV(&&)
ESEED_VEC_BIN_SV(||)
#undef ESEED_VEC_BIN_SV

// Assignment vector-vector
#define ESEED_VEC_ASSN_VV(op)                                                               \
    template <std::size_t L, typename T0, typename T1, typename = decltype(T0(0) op T1(0))> \
    Vec<L, T0>& operator op##=(Vec<L, T0>& a, const Vec<L, T1>& b) {                        \
        for (std::size_t i = 0; i < L; i++) a[i] op##= b[i];                                \
        return a;                                                                           \
    }
ESEED_VEC_ASSN_VV(+)
ESEED_VEC_ASSN_VV(-)
ESEED_VEC_ASSN_VV(*)
ESEED_VEC_ASSN_VV(/)
ESEED_VEC_ASSN_VV(%)
ESEED_VEC_ASSN_VV(&)
ESEED_VEC_ASSN_VV(|)
ESEED_VEC_ASSN_VV(^)
ESEED_VEC_ASSN_VV(<<)
ESEED_VEC_ASSN_VV(>>)
#undef ESEED_VEC_ASSN_VV

// Assignment vector-scalar
#define ESEED_VEC_ASSN_VS(op)                                                               \
    template <std::size_t L, typename T0, typename T1, typename = decltype(T0(0) op T1(0))> \
    Vec<L, T0>& operator op##=(Vec<L, T0>& a, T1 b) {                                       \
        for (std::size_t i = 0; i < L; i++) a[i] op##= b;                                   \
        return a;                                                                           \
    }
ESEED_VEC_ASSN_VS(+)
ESEED_VEC_ASSN_VS(-)
ESEED_VEC_ASSN_VS(*)
ESEED_VEC_ASSN_VS(/)
ESEED_VEC_ASSN_VS(%)
ESEED_VEC_ASSN_VS(&)
ESEED_VEC_ASSN_VS(|)
ESEED_VEC_ASSN_VS(^)
ESEED_VEC_ASSN_VS(<<)
ESEED_VEC_ASSN_VS(>>)
#undef ESEED_VEC_ASSN_VS

}