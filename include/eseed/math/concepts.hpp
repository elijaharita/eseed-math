#pragma once

#include <type_traits>

namespace esdm {

template <typename T>
concept AnyFloat = std::is_floating_point_v<T>;

template <typename T>
concept AnyNum = std::is_arithmetic_v<T>;

template <typename T>
concept AnyInt = std::is_integral_v<T>;

template <typename T>
concept AnySigned = std::is_signed_v<T>;

template <typename T>
concept AnyUnsigned = std::is_unsigned_v<T>;

template <typename From, typename To>
concept ConvertibleTo = std::is_convertible_v<From, To>;

}