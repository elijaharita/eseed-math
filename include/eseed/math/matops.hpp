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

#pragma once

#include "mat.hpp"
#include "ops.hpp"

namespace esd::math {

// -- GENERAL FUNCTIONS -- //

// Transpose of a matrix
template <std::size_t M, std::size_t N, typename T>
constexpr Mat<N, M, T> transpose(const Mat<M, N, T> m) {
    Mat<M, N, T> out;
    for (std::size_t i = 0; i < M; i++)
        for (std::size_t j = 0; j < N; j++)
            out[i][j] = m[j][i];
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

namespace esdm = esd::math;