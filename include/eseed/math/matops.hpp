#pragma once

#include "mat.hpp"
#include "ops.hpp"

namespace esdm {

template <typename T>
Mat4<T> matTranslate(const Vec3<T> &translation) {

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

template <typename T>
Mat4<T> matRotate(const Vec3<T>& axis, T angle) {

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

template <typename T>
Mat4<T> rotateX(const T &xAngle) {
    return Mat4<T> {
        1, 0, 0, 0,
        0, cos(xAngle), -sin(xAngle), 0,
        0, sin(xAngle), cos(xAngle), 0,
        0, 0, 0, 1
    };
}

template <typename T>
Mat4<T> rotateY(const T &yAngle) {
    return Mat4<T> {
        cos(yAngle), 0, sin(yAngle), 0,
        0, 1, 0, 0,
        -sin(yAngle), 0, cos(yAngle), 0,
        0, 0, 0, 1
    };
}

template <typename T>
Mat4<T> rotateZ(const T &zAngle) {
    return Mat4<T> {
        cos(zAngle), -sin(zAngle), 0, 0,
        sin(zAngle), cos(zAngle), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
}

};