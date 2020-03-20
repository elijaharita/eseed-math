#pragma once

#include "mat.hpp"
#include "ops.hpp"

namespace esdm {

// Generate translation matrix from offset
template <typename T>
Mat4<T> mtrans(const Vec3<T> &translation) {

    constexpr T n0 = T(0);
    constexpr T n1 = T(1);
    const T nx = translation.x;
    const T ny = translation.y;
    const T nz = translation.z;
    
    return Mat4<T> {
        n1, n0, n0, n0,
        n0, n1, n0, n0,
        n0, n0, n1, n0,
        nx, ny, nz, n1
    };
}

// Generate rotation matrix from axis and angle
template <typename T>
Mat4<T> mrot(const Vec3<T>& axis, T angle) {

    const T c = cos(angle);
    const T s = sin(angle);
    const T t = 1 - c;
    const T x = axis.x;
    const T y = axis.y;
    const T z = axis.z;

    constexpr T n0 = 0;
    constexpr T n1 = 1;
    const T xx = t * x * x + c;
    const T xy = t * x * y + z * s;
    const T xz = t * x * z - y * s;
    const T yx = t * x * y - z * s;
    const T yy = t * y * y + c;
    const T yz = t * y * z + x * s;
    const T zx = t * x * z + y * s;
    const T zy = t * y * z - x * s;
    const T zz = t * z * z + c;

    return Mat4<T> {
        xx, xy, xz, n0,
        yx, yy, yz, n0,
        zx, zy, zz, n0,
        n0, n0, n0, n1
    };
}

};