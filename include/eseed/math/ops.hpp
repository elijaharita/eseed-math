#pragma once

#include <cstddef>
#include <algorithm>
#include <type_traits>

namespace esdm {

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
constexpr T pi() {
    return T(3.14159265358979323846);
}

template <typename T>
inline T abs(T n) {
    return n < 0 ? -n : n;
}

template <typename T>
inline T trunc(T n) {
    return std::trunc(n);
}

template <typename T>
inline T floor(T n) {
    return std::floor(n);
}

template <typename T>
inline T ceil(T n) {
    return std::ceil(n);
}

template <typename T>
inline T round(T n) {
    return std::round(n);
}

// "i" functions perform the operation directly to an integer type
// Considerably faster that normal floating point operations when
// special floating point behavior is not needed
// Warning: NaN becomes 0, Inifinity becomes 1, -Infinity becomes -1

template <typename I, typename T, typename = std::enable_if_t<std::is_integral_v<I>>>
inline I itrunc(T n) {
    return (I)n;
}

template <typename I, typename T, typename = std::enable_if_t<std::is_integral_v<I>>>
inline I ifloor(T n) {
    I ni = (I)n;
    return n < ni ? ni - 1 : ni;
}

template <typename I, typename T, typename = std::enable_if_t<std::is_integral_v<I>>>
inline I iceil(T n) {
    I ni = (I)n;
    return n > ni ? ni + 1 : ni;
}

template <typename I, typename T, typename = std::enable_if_t<std::is_integral_v<I>>>
inline I iround(T n) {
    I ni = (I)n;
    return n > 0 ? (n - ni >= 0.5 ? ni + 1 : ni) : (n - ni <= -0.5 ? ni - 1 : ni);
}

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
inline T sin(T n) {
    return std::sin(n);
}

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
inline T cos(T n) {
    return std::cos(n);
}

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
inline T tan(T n) {
    return std::tan(n);
}

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
inline T asin(T n) {
    return std::asin(n);
}

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
inline T acos(T n) {
    return std::acos(n);
}

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
inline T atan(T n) {
    return std::atan(n);
}

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
inline T atan2(T y, T x) {
    return std::atan2(y, x);
}

}