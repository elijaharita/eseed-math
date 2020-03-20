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

template <size_t L, typename T>
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
template <size_t L, typename T>
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

template <size_t L, typename T>
class Vec : public VecData<L, T> {
public:
    // Vec<3, T>(): [ 0, 0, 0 ]
    constexpr Vec() : VecData<L, T>{0} {}

    // Vec<3, T>(x, y, z) => [ x, y, z ]
    // Vec<3, T>(x, y) => [ x, y, 0 ]
    template <ConvertibleTo<T>... Ts> requires (sizeof...(Ts) <= L)
    constexpr Vec(const Ts&... components) : VecData<L, T>{((T)components)...} {}

    // Vec<3, T>(n) => [ n, n, n ]
    explicit constexpr Vec(const T& n) {
        for (size_t i = 0; i < L; i++) data[i] = n;
    }

    // Vec<3, T>(/*Vec<2, U>*/ other) => [ (T)other.x, (T)other.y, 0 ]
    template <ConvertibleTo<T> T1, size_t L1>
    explicit constexpr Vec(const Vec<L1, T1>& other) {
        for (size_t i = 0; i < std::min(L, L1); i++) data[i] = (T)other[i];
    }

    constexpr T operator[](size_t i) const {
        return data[i];
    }

    constexpr T& operator[](size_t i) {
        return data[i];
    }
};

// Printing

template <size_t L, typename T>
std::ostream& operator<<(std::ostream& out, const Vec<L, T>& v) {
    out << "[";
    for (size_t i = 0; i < L; i++) {
        out << v[i];
        if (i < L - 1)
            out << ", ";
    }
    out << "]";
    return out;
}

// Operators

#define ESEED_VEC_CMPR()                                                               \
    template <size_t L, typename T0, typename T1, typename = decltype(T0(0) == T1(0))> \
    constexpr bool operator==(const Vec<L, T0>& a, const Vec<L, T1>& b) {              \
        for (size_t i = 0; i < L; i++) if (a[i] != b[i]) return false;                 \
        return true;                                                                   \
    }                                                                                  \

#define ESEED_VEC_PRE(op)                                                 \
    template <size_t L, typename T, typename = decltype((*(new T(0)))op)> \
    constexpr Vec<L, T>& operator op(Vec<L, T>& v) {                      \
        for (size_t i = 0; i < L; i++) op v[i];                           \
        return v;                                                         \
    }

#define ESEED_VEC_POST(op)                                                \
    template <size_t L, typename T, typename = decltype((*(new T(0)))op)> \
    constexpr Vec<L, T> operator op(Vec<L, T>& v, int) {                  \
        Vec<L, T> out = v;                                                \
        for (size_t i = 0; i < L; i++) v[i] op;                           \
        return out;                                                       \
    }

#define ESEED_VEC_UN(op)                                          \
    template <size_t L, typename T, typename = decltype(op T(0))> \
    constexpr Vec<L, T> operator op(const Vec<L, T>& v) {         \
        Vec<L, T> out;                                            \
        for (size_t i = 0; i < L; i++) out[i] = op v[i];          \
        return out;                                               \
    }

#define ESEED_VEC_BIN_VV(op)                                                                \
    template <size_t L, typename T0, typename T1, typename TRes = decltype(T0(0) op T1(0))> \
    constexpr Vec<L, TRes> operator op(const Vec<L, T0>& a, const Vec<L, T1>& b) {          \
        Vec<L, TRes> out;                                                                   \
        for (size_t i = 0; i < L; i++) out[i] = a[i] op b[i];                               \
        return out;                                                                         \
    }

#define ESEED_VEC_BIN_VS(op)                                                                \
    template <size_t L, typename T0, typename T1, typename TRes = decltype(T0(0) op T1(0))> \
    constexpr Vec<L, TRes> operator op(const Vec<L, T0>& a, T1 b) {                         \
        Vec<L, TRes> out;                                                                   \
        for (size_t i = 0; i < L; i++) out[i] = a[i] op b;                                  \
        return out;                                                                         \
    }

#define ESEED_VEC_BIN_SV(op)                                                                \
    template <size_t L, typename T0, typename T1, typename TRes = decltype(T0(0) op T1(0))> \
    constexpr Vec<L, TRes> operator op(const T0& a, const Vec<L, T1>& b) {                  \
        Vec<L, TRes> out;                                                                   \
        for (size_t i = 0; i < L; i++) out[i] = a op b[i];                                  \
        return out;                                                                         \
    }

#define ESEED_VEC_ASSN_VV(op)                                                          \
    template <size_t L, typename T0, typename T1, typename = decltype(T0(0) op T1(0))> \
    Vec<L, T0>& operator op##=(Vec<L, T0>& a, const Vec<L, T1>& b) {                   \
        for (size_t i = 0; i < L; i++) a[i] op##= b[i];                                \
        return a;                                                                      \
    }

#define ESEED_VEC_ASSN_VS(op)                                                          \
    template <size_t L, typename T0, typename T1, typename = decltype(T0(0) op T1(0))> \
    Vec<L, T0>& operator op##=(Vec<L, T0>& a, T1 b) {                                  \
        for (size_t i = 0; i < L; i++) a[i] op##= b;                                   \
        return a;                                                                      \
    }

ESEED_VEC_CMPR()

ESEED_VEC_PRE(++)
ESEED_VEC_PRE(--)

ESEED_VEC_POST(--)
ESEED_VEC_POST(++)

ESEED_VEC_UN(+)
ESEED_VEC_UN(-)
ESEED_VEC_UN(!)
ESEED_VEC_UN(~)

ESEED_VEC_BIN_VV(+)
ESEED_VEC_BIN_VV(-)
ESEED_VEC_BIN_VV(*)
ESEED_VEC_BIN_VV(/)
ESEED_VEC_BIN_VV(%)
ESEED_VEC_BIN_VV(&)
ESEED_VEC_BIN_VV(|)
ESEED_VEC_BIN_VV (^)
ESEED_VEC_BIN_VV(<<)
ESEED_VEC_BIN_VV(>>)
ESEED_VEC_BIN_VV(&&)
ESEED_VEC_BIN_VV(||)

ESEED_VEC_BIN_VS(+)
ESEED_VEC_BIN_VS(-)
ESEED_VEC_BIN_VS(*)
ESEED_VEC_BIN_VS(/)
ESEED_VEC_BIN_VS(%)
ESEED_VEC_BIN_VS(&)
ESEED_VEC_BIN_VS(|)
ESEED_VEC_BIN_VS (^)
ESEED_VEC_BIN_VS(<<)
ESEED_VEC_BIN_VS(>>)
ESEED_VEC_BIN_VS(&&)
ESEED_VEC_BIN_VS(||)

ESEED_VEC_BIN_SV(+)
ESEED_VEC_BIN_SV(-)
ESEED_VEC_BIN_SV(*)
ESEED_VEC_BIN_SV(/)
ESEED_VEC_BIN_SV(%)
ESEED_VEC_BIN_SV(&)
ESEED_VEC_BIN_SV(|)
ESEED_VEC_BIN_SV (^)
ESEED_VEC_BIN_SV(<<)
ESEED_VEC_BIN_SV(>>)
ESEED_VEC_BIN_SV(&&)
ESEED_VEC_BIN_SV(||)

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

}