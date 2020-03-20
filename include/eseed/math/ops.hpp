#pragma once

#include <eseed/math/concepts.hpp>
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <type_traits>
#include <cmath>
#include <concepts>

namespace esdm {

// -- SPECIAL FLOATING POINT VALUES -- //

// Infinity
template <AnyFloat T>
constexpr T inf() {
    return std::numeric_limits<T>::infinity();
}

// Check for positive or negative infinity
template <AnyFloat T>
constexpr bool isinf(T n) {
    return n == inf<T>() || n == -inf<T>();
}

// Quiet NaN
template <AnyFloat T>
constexpr T qnan() {
    return std::numeric_limits<T>::quiet_NaN();
}

// Signaling NaN
template <AnyFloat T>
constexpr T snan() {
    return std::numeric_limits<T>::signaling_NaN();
}

// Alias for quiet NaN
template <AnyFloat T>
constexpr T nan() {
    return qnan<T>();
}

// Check for NaN
template <AnyFloat T>
constexpr bool isnan(T n) {
    return n != n;
}

// -- CONSTANTS -- //

// Pi
template <AnyFloat T>
constexpr T pi() {
    return T(3.14159265358979323846);
}

// -- GENERAL FUNCTIONS -- //

// Absolute value
template <AnyNum T>
constexpr T abs(T n) {
    return n < T(0) ? -n : n;
}

// Square
template <AnyNum T>
constexpr T sq(T n) {
    return n * n;
}

// Square root
template <AnyNum T>
inline T sqrt(T n) {
    return std::sqrt(n);
}

// Power
template <AnyNum T0, AnyNum T1>
inline std::common_type_t<T0, T1> pow(T0 b, T1 e) {
    return std::pow(b, e);
}

// -- ROUNDING -- //

// Truncate
template <AnyFloat T>
inline T trunc(T n) {
    return std::trunc(n);
}

// Floor
template <AnyFloat T>
inline T floor(T n) {
    return std::floor(n);
}

// Ceil
template <AnyFloat T>
inline T ceil(T n) {
    return std::ceil(n);
}

// Round
template <AnyFloat T>
inline T round(T n) {
    return std::round(n);
}

// -- INTEGER ROUNDING -- //

// "i" functions perform the operation directly to an integer type
// Considerably faster that normal floating point operations when
// special floating point behavior is not needed
// Warning: NaN becomes 0, Inifinity becomes 1, -Infinity becomes -1

// Direct-to-int truncate
template <AnyInt I, AnyFloat T>
constexpr I itrunc(T n) {
    return (I)n;
}

// Direct-to-int floor
template <AnyInt I, AnyFloat T>
constexpr I ifloor(T n) {
    I ni = (I)n;
    return n < ni ? ni - 1 : ni;
}

// Direct-to-int ceil
template <AnyInt I, AnyFloat T>
constexpr I iceil(T n) {
    I ni = (I)n;
    return n > ni ? ni + 1 : ni;
}

// Direct-to-int round
template <AnyInt I, AnyFloat T>
constexpr I iround(T n) {
    I ni = (I)n;
    return n > 0 
        ? (n - ni >= 0.5 ? ni + 1 : ni) 
        : (n - ni <= -0.5 ? ni - 1 : ni);
}

// -- TRIGONOMETRY -- //

// Sine
template <AnyFloat T>
constexpr T sin(T n) {
    return std::sin(n);
}

// Cosine
template <AnyFloat T>
constexpr T cos(T n) {
    return std::cos(n);
}

// Tangent
template <AnyFloat T>
constexpr T tan(T n) {
    return std::tan(n);
}

// Arcsine
template <AnyFloat T>
constexpr T asin(T n) {
    return std::asin(n);
}

// Arccosine
template <AnyFloat T>
constexpr T acos(T n) {
    return std::acos(n);
}

// Arctangent
template <AnyFloat T>
constexpr T atan(T n) {
    return std::atan(n);
}

// Two-argument arctangent
template <AnyFloat T>
constexpr T atan2(T y, T x) {
    return std::atan2(y, x);
}

// Hyperbolic sine
template <AnyFloat T>
constexpr T sinh(T n) {
    return std::sinh(n);
}

// Hyperbolic cosine
template <AnyFloat T>
constexpr T cosh(T n) {
    return std::cosh(n);
}

// Hyperbolic tangent
template <AnyFloat T>
constexpr T tanh(T n) {
    return std::tanh(n);
}

// Hyperbolic arcsine
template <AnyFloat T>
constexpr T asinh(T n) {
    return std::asinh(n);
}

// Hyperbolic arccosine
template <AnyFloat T>
constexpr T acosh(T n) {
    return std::acosh(n);
}

// Hyperbolic arctangent
template <AnyFloat T>
constexpr T atanh(T n) {
    return std::atanh(n);
}

}