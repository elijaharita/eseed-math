#pragma once

#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <type_traits>
#include <cmath>

namespace esdm {

template <typename T0, typename T1> 
using PowT = decltype(std::pow(T0(0), T1(0)));

// -- SPECIAL FLOATING POINT VALUES -- //

// Infinity
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
constexpr T inf() {
    return std::numeric_limits<T>::infinity();
}

// Check for positive or negative infinity
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
constexpr bool isinf(T n) {
    return n == inf<T>() || n == -inf<T>();
}

// Quiet NaN
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
constexpr T qnan() {
    return std::numeric_limits<T>::quiet_NaN();
}

// Signaling NaN
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
constexpr T snan() {
    return std::numeric_limits<T>::signaling_NaN();
}

// Alias for quiet NaN
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
constexpr T nan() {
    return qnan<T>();
}

// Check for NaN
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
constexpr bool isnan(T n) {
    return n != n;
}

// -- CONSTANTS -- //

// Pi
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
constexpr T pi() {
    return T(3.14159265358979323846);
}

// -- GENERAL FUNCTIONS -- //

// Absolute value
template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr T abs(T n) {
    return n < T(0) ? -n : n;
}

// Square
template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr T sq(T n) {
    return n * n;
}

// Square root
template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline T sqrt(T n) {
    return std::sqrt(n);
}

// Power
template <typename T0, typename T1>
inline PowT<T0, T1> pow(T0 b, T1 e) {
    return std::pow(b, e);
}

// -- ROUNDING -- //

// Truncate
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
inline T trunc(T n) {
    return std::trunc(n);
}

// Floor
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
inline T floor(T n) {
    return std::floor(n);
}

// Ceil
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
inline T ceil(T n) {
    return std::ceil(n);
}

// Round
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
inline T round(T n) {
    return std::round(n);
}

// -- INTEGER ROUNDING -- //

// "i" functions perform the operation directly to an integer type
// Considerably faster that normal floating point operations when
// special floating point behavior is not needed
// Warning: NaN becomes 0, Inifinity becomes 1, -Infinity becomes -1

// Direct-to-int truncate
template <typename I, typename T, typename = std::enable_if_t<std::is_integral_v<I>>>
constexpr I itrunc(T n) {
    return (I)n;
}

// Direct-to-int floor
template <typename I, typename T, typename = std::enable_if_t<std::is_integral_v<I>>>
constexpr I ifloor(T n) {
    I ni = (I)n;
    return n < ni ? ni - 1 : ni;
}

// Direct-to-int ceil
template <typename I, typename T, typename = std::enable_if_t<std::is_integral_v<I>>>
constexpr I iceil(T n) {
    I ni = (I)n;
    return n > ni ? ni + 1 : ni;
}

// Direct-to-int round
template <typename I, typename T, typename = std::enable_if_t<std::is_integral_v<I>>>
constexpr I iround(T n) {
    I ni = (I)n;
    return n > 0 ? (n - ni >= 0.5 ? ni + 1 : ni) : (n - ni <= -0.5 ? ni - 1 : ni);
}

// -- TRIGONOMETRY -- //

// Sine
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
constexpr T sin(T n) {
    return std::sin(n);
}

// Cosine
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
constexpr T cos(T n) {
    return std::cos(n);
}

// Tangent
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
constexpr T tan(T n) {
    return std::tan(n);
}

// Arcsine
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
constexpr T asin(T n) {
    return std::asin(n);
}

// Arccosine
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
constexpr T acos(T n) {
    return std::acos(n);
}

// Arctangent
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
constexpr T atan(T n) {
    return std::atan(n);
}

// Two-argument arctangent
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
constexpr T atan2(T y, T x) {
    return std::atan2(y, x);
}

}