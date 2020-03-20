#pragma once

#include "ops.hpp"

#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <ostream>
#include <array>
#include <algorithm>

namespace esdm {

template <size_t L, typename T>
class VecData {
public:
    std::array<T, L> data;
};

template <typename T>
class VecData<1, T> {
public:
    union {
        std::array<T, 1> data;
        T x;
        T r;
    };
};

template <typename T>
class VecData<2, T> {
public:
    union {
        std::array<T, 2> data;
        struct { T x, y; };
        struct { T r, g; };
    };
};

template <typename T>
class VecData<3, T> {
public:
    union {
        std::array<T, 3> data;
        struct { T x, y, z; };
        struct { T r, g, b; };
    };
};

template <typename T>
class VecData<4, T> {
public:
    union {
        std::array<T, 4> data;
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
    Vec() : VecData<L, T>{0} {}

    // Vec<3, T>(arr) => [ arr[0], arr[1], arr[2] ]
    Vec(const T* data) {
        std::copy(data, data + L, this->data.data());
    }

    // Vec<3, T>(a, b, c) => [ a, b, c ]
    template <typename... Ts, typename std::enable_if_t<std::conjunction_v<std::is_convertible<Ts, T>...> && (sizeof...(Ts) == L)>* = nullptr>
    Vec(const Ts&... components) : VecData<L, T>{((T)components)...} {}

    // Vec<3, T>(v) => [ v, v, v ]
    explicit Vec(const T& component) {
        for (size_t i = 0; i < L; i++)
            data[i] = component;
    }

    // Vec<3, T>(/*Vec<2, U>*/ other) => [ (T)other.x, (T)other.y, 0 ]
    template <typename T1, size_t L1>
    explicit Vec(const Vec<L1, T1>& other) {
        for (size_t i = 0; i < std::min(L, L1); i++) data[i] = (T)other[i];
    }

    // Vec<3, T>(/*Vec<3, U>*/ other) => [ (T)other.x, (T)other.y, (T)other.z ]
    template <typename T1>
    constexpr Vec(const Vec<L, T1>& other) {
        for (size_t i = 0; i < L; i++) data[i] = (T)other[i];
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
    bool operator==(const Vec<L, T0>& a, const Vec<L, T1>& b) {                        \
        for (size_t i = 0; i < L; i++) if (a[i] != b[i]) return false;                 \
        return true;                                                                   \
    }                                                                                  \

#define ESEED_VEC_PRE(op)                                                 \
    template <size_t L, typename T, typename = decltype((*(new T(0)))op)> \
    Vec<L, T>& operator op(Vec<L, T>& v) {                                \
        for (size_t i = 0; i < L; i++) op v[i];                           \
        return v;                                                         \
    }

#define ESEED_VEC_POST(op)                                                \
    template <size_t L, typename T, typename = decltype((*(new T(0)))op)> \
    Vec<L, T> operator op(Vec<L, T>& v, int) {                            \
        Vec<L, T> out = v;                                                \
        for (size_t i = 0; i < L; i++) v[i] op;                           \
        return out;                                                       \
    }

#define ESEED_VEC_UN(op)                                          \
    template <size_t L, typename T, typename = decltype(op T(0))> \
    Vec<L, T> operator op(const Vec<L, T>& v) {                   \
        Vec<L, T> out;                                            \
        for (size_t i = 0; i < L; i++) out[i] = op v[i];          \
        return out;                                               \
    }

#define ESEED_VEC_BIN_VV(op)                                                                \
    template <size_t L, typename T0, typename T1, typename TRes = decltype(T0(0) op T1(0))> \
    Vec<L, TRes> operator op(const Vec<L, T0>& a, const Vec<L, T1>& b) {                    \
        Vec<L, TRes> out;                                                                   \
        for (size_t i = 0; i < L; i++) out[i] = a[i] op b[i];                               \
        return out;                                                                         \
    }

#define ESEED_VEC_BIN_VS(op)                                                                \
    template <size_t L, typename T0, typename T1, typename TRes = decltype(T0(0) op T1(0))> \
    Vec<L, TRes> operator op(const Vec<L, T0>& a, T1 b) {                                   \
        Vec<L, TRes> out;                                                                   \
        for (size_t i = 0; i < L; i++) out[i] = a[i] op b;                                  \
        return out;                                                                         \
    }

#define ESEED_VEC_BIN_SV(op)                                                                \
    template <size_t L, typename T0, typename T1, typename TRes = decltype(T0(0) op T1(0))> \
    Vec<L, TRes> operator op(const T0& a, const Vec<L, T1>& b) {                            \
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

ESEED_VEC_UN(!)
ESEED_VEC_UN(~)
ESEED_VEC_UN(-)
ESEED_VEC_UN(+)

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

// Functions

template <size_t L, typename T>
Vec<L, T> abs(const Vec<L, T>& v) {
    Vec<L, T> out;
    for (size_t i = 0; i < L; i++)
        out[i] = abs(v[i]);
    return out;
}

template <size_t L, typename T>
Vec<L, T> trunc(const Vec<L, T>& v) {
    Vec<L, T> out;
    for (size_t i = 0; i < L; i++)
        out[i] = trunc(v[i]);
    return out;
}

template <size_t L, typename T>
Vec<L, T> floor(const Vec<L, T>& v) {
    Vec<L, T> out;
    for (size_t i = 0; i < L; i++)
        out[i] = floor(v[i]);
    return out;
}

template <size_t L, typename T>
Vec<L, T> ceil(const Vec<L, T>& v) {
    Vec<L, T> out;
    for (size_t i = 0; i < L; i++)
        out[i] = ceil(v[i]);
    return out;
}

template <size_t L, typename T>
Vec<L, T> round(const Vec<L, T>& v) {
    Vec<L, T> out;
    for (size_t i = 0; i < L; i++)
        out[i] = round(v[i]);
    return out;
}

// See ops.hpp for "i" functions explanation

template <typename I, size_t L, typename T>
Vec<L, I> itrunc(const Vec<L, T>& v) {
    Vec<L, I> out;
    for (size_t i = 0; i < L; i++)
        out[i] = itrunc<I>(v[i]);
    return out;
}

template <typename I, size_t L, typename T>
Vec<L, I> ifloor(const Vec<L, T>& v) {
    Vec<L, I> out;
    for (size_t i = 0; i < L; i++)
        out[i] = ifloor<I>(v[i]);
    return out;
}

template <typename I, size_t L, typename T>
Vec<L, I> iceil(const Vec<L, T>& v) {
    Vec<L, I> out;
    for (size_t i = 0; i < L; i++)
        out[i] = iceil<I>(v[i]);
    return out;
}

template <typename I, size_t L, typename T>
Vec<L, I> iround(const Vec<L, T>& v) {
    Vec<L, I> out;
    for (size_t i = 0; i < L; i++)
        out[i] = iround<I>(v[i]);
    return out;
}

template <size_t L, typename T0, typename T1>
decltype(T0(0) * T1(0)) dot(const Vec<L, T0>& a, const Vec<L, T1>& b) {
    decltype(T0(0) * T1(0)) out = 0;
    for (size_t i = 0; i < L; i++)
        out += a[i] * b[i];
    return out;
}

template <typename T0, typename T1>
Vec3<decltype(T0(0) * T1(0))> cross(const Vec3<T0>& a, const Vec3<T1>& b) {
    return Vec3<decltype(T0(0) * T1(0))>(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x);
}

}