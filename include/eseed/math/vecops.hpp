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

#include "vec.hpp"

namespace esdm {

// -- SPECIAL FLOATING POINT VALUES -- //

// Check if all components are infinity
template <size_t L, AnyFloat T>
constexpr bool allinf(const Vec<L, T>& v) {
    for (size_t i = 0; i < L; i++) if (!isinf(v[i])) return false;
    return true;
}

// Check if any components are infinity
template <size_t L, AnyFloat T>
constexpr bool anyinf(const Vec<L, T>& v) {
    for (size_t i = 0; i < L; i++) if (isinf(v[i])) return true;
    return false;
}

// Check if all components are NaN
template <size_t L, AnyFloat T>
constexpr bool allnan(const Vec<L, T>& v) {
    for (size_t i = 0; i < L; i++) if (!isnan(v[i])) return false;
    return true;
}

// Check if any components are NaN
template <size_t L, AnyFloat T>
constexpr bool anynan(const Vec<L, T>& v) {
    for (size_t i = 0; i < L; i++) if (isnan(v[i])) return true;
    return false;
}

// -- GENERAL FUNCTIONS -- //

// Absolute value all components
template <size_t L, AnyNum T>
constexpr Vec<L, T> abs(const Vec<L, T>& v) {
    Vec<L, T> out;
    for (size_t i = 0; i < L; i++) out[i] = abs(v[i]);
    return out;
}

// Square all components
template <size_t L, AnyNum T>
constexpr Vec<L, T> sq(const Vec<L, T>& v) {
    Vec<L, T> out;
    for (size_t i = 0; i < L; i++) out[i] = sq(v[i]);
    return out;
}

// Square root all components
template <size_t L, AnyNum T>
inline Vec<L, T> sqrt(const Vec<L, T>& v) {
    Vec<L, T> out;
    for (size_t i = 0; i < L; i++) out[i] = sqrt(v[i]);
    return out;
}

// Raise components to a power
template <size_t L, AnyNum T0, AnyNum T1>
constexpr Vec<L, std::common_type_t<T0, T1>> pow(const Vec<L, T0>& b, T1 e) {
    Vec<L, T> out;
    for (size_t i = 0; i < L; i++) out[i] = sq(v[i]);
    return out;
}

// Dot product
template <size_t L, AnyNum T0, AnyNum T1>
constexpr std::common_type_t<T0, T1> dot(
    const Vec<L, T0>& a, 
    const Vec<L, T1>& b
) {
    decltype(T0(0) * T1(0)) out = 0;
    for (size_t i = 0; i < L; i++) out += a[i] * b[i];
    return out;
}

// Cross product
template <AnyNum T0, AnyNum T1>
constexpr Vec3<std::common_type_t<T0, T1>> cross(
    const Vec3<T0>& a, 
    const Vec3<T1>& b
) {
    return Vec3<decltype(T0(0) * T1(0))>(
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0]
    );
}

// -- ROUNDING -- //

// Truncate all components
template <size_t L, AnyFloat T>
inline Vec<L, T> trunc(const Vec<L, T>& v) {
    Vec<L, T> out;
    for (size_t i = 0; i < L; i++) out[i] = trunc(v[i]);
    return out;
}

// Floor all components
template <size_t L, AnyFloat T>
inline Vec<L, T> floor(const Vec<L, T>& v) {
    Vec<L, T> out;
    for (size_t i = 0; i < L; i++) out[i] = floor(v[i]);
    return out;
}

// Ceil all components
template <size_t L, AnyFloat T>
inline Vec<L, T> ceil(const Vec<L, T>& v) {
    Vec<L, T> out;
    for (size_t i = 0; i < L; i++) out[i] = ceil(v[i]);
    return out;
}

// Round all components
template <size_t L, AnyFloat T>
inline Vec<L, T> round(const Vec<L, T>& v) {
    Vec<L, T> out;
    for (size_t i = 0; i < L; i++) out[i] = round(v[i]);
    return out;
}

// -- DIRECT-TO-INT ROUNDING -- //

// See ops.hpp for "i" functions explanation

// Direct-to-int truncate all components
template <AnyInt I, size_t L, AnyFloat T>
constexpr Vec<L, I> itrunc(const Vec<L, T>& v) {
    Vec<L, I> out;
    for (size_t i = 0; i < L; i++) out[i] = itrunc<I>(v[i]);
    return out;
}

// Direct-to-int floor all components
template <AnyInt I, size_t L, AnyFloat T>
constexpr Vec<L, I> ifloor(const Vec<L, T>& v) {
    Vec<L, I> out;
    for (size_t i = 0; i < L; i++) out[i] = ifloor<I>(v[i]);
    return out;
}

// Direct-to-int ceil all components
template <AnyInt I, size_t L, AnyFloat T>
constexpr Vec<L, I> iceil(const Vec<L, T>& v) {
    Vec<L, I> out;
    for (size_t i = 0; i < L; i++) out[i] = iceil<I>(v[i]);
    return out;
}

// Direct-to-int round all components
template <AnyInt I, size_t L, AnyFloat T>
constexpr Vec<L, I> iround(const Vec<L, T>& v) {
    Vec<L, I> out;
    for (size_t i = 0; i < L; i++) out[i] = iround<I>(v[i]);
    return out;
}

}