#pragma once

#include "mat.hpp"
#include "ops.hpp"

namespace esdm {

// -- GENERAL FUNCTIONS -- //

// Inverse of a matrix
template <size_t M, size_t N, typename T>
constexpr Mat<N, M, T> inverse(const Mat<M, N, T> m) {
    Mat<M, N, T> out;
    for (size_t i = 0; i < M; i++)
        for (size_t j = 0; j < N; j++)
            out[i][j] = m[j][i];
    return out;
}

// Multiply two matrices
template <size_t M, size_t N, size_t MN, typename T0, typename T1>
Mat<M, N, decltype(T0(0) * T1(0))> matmul(const Mat<M, MN, T0> &a, const Mat<MN, N, T1> &b) {
    Mat<M, N, decltype(T0(0) * T1(0))> out;
    for (size_t i = 0; i < N; i++)
        for (size_t j = 0; j < M; j++)
            out[i][j] = dot(a.getRow(i), b.getCol(j));
    return out;
}

// Multiply a matrix by a row vector
template <size_t M, size_t N, typename T0, typename T1>
Vec<M, decltype(T0(0), T1(0))> matmul(const Mat<M, N, T0> &a, const Vec<N, T1> &b) {
    Vec<M, decltype(T0(0) * T1(0))> out;
    for (size_t i = 0; i < M; i++)
        out[i] = dot(a.getRow(i), b);
    return out;
}

// Multiply a column vector by a matrix
template <size_t M, size_t N, typename T0, typename T1>
Vec<N, decltype(T0(0), T1(0))> matmul(const Vec<M, T0> &a, const Mat<M, N, T1> &b) {
    Vec<N, decltype(T0(0) * T1(0))> out;
    for (size_t j = 0; j < N; j++)
        out[j] = dot(a, b.getCol(j));
    return out;
}

// -- MATRIX GENERATION -- //

// Generate translation matrix from offset
template <typename T>
constexpr Mat4<T> mattrans(const Vec3<T> &translation) {

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
constexpr Mat4<T> matrot(const Vec3<T>& axis, T angle) {

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