#pragma once

#include "vec.hpp"

#include <ostream>
#include <iostream>
#include <array>

namespace esdm {

template <size_t M, size_t N, typename T>
class MatData
{
public:
    std::array<Vec<N, T>, M> data;
};

// Forward declaration for shorthand aliases
template <size_t M, size_t N, typename T>
class Mat;

// Shorthand aliases

template <typename T>
using Mat1x1 = Mat<1, 1, T>;

template <typename T>
using Mat1x2 = Mat<1, 2, T>;

template <typename T>
using Mat1x3 = Mat<1, 3, T>;

template <typename T>
using Mat1x4 = Mat<1, 4, T>;

template <typename T>
using Mat2x1 = Mat<2, 1, T>;

template <typename T>
using Mat2x2 = Mat<2, 2, T>;

template <typename T>
using Mat2x3 = Mat<2, 3, T>;

template <typename T>
using Mat2x4 = Mat<2, 4, T>;

template <typename T>
using Mat3x1 = Mat<3, 1, T>;

template <typename T>
using Mat3x2 = Mat<3, 2, T>;

template <typename T>
using Mat3x3 = Mat<3, 3, T>;

template <typename T>
using Mat3x4 = Mat<3, 4, T>;

template <typename T>
using Mat4x1 = Mat<4, 1, T>;

template <typename T>
using Mat4x2 = Mat<4, 2, T>;

template <typename T>
using Mat4x3 = Mat<4, 3, T>;

template <typename T>
using Mat4x4 = Mat<4, 4, T>;

template <typename T>
using Mat1 = Mat1x1<T>;

template <typename T>
using Mat2 = Mat2x2<T>;

template <typename T>
using Mat3 = Mat3x3<T>;

template <typename T>
using Mat4 = Mat4x4<T>;

template <size_t M, size_t N, typename T>
class Mat : public MatData<M, N, T> {
public:
    using Col = Vec<M, T>;
    using Row = Vec<N, T>;

    // Mat<2, 2, T>() =>
    // [ 0, 0 ]
    // [ 0, 0 ]
    Mat() {}

    // Mat<2, 2, T>(arr) =>
    // [ arr[0], arr[2] ]
    // [ arr[1], arr[3] ]
    Mat(const T* arr) {

        std::copy(arr, arr + M * N, &data[0][0]);
    }

    // Mat<2, 2, T>(a, b, c, d) =>
    // [ a, c ]
    // [ b, d ]
    template <typename... Ts, typename std::enable_if_t<std::conjunction_v<std::is_same<Ts, T>...> && (sizeof...(Ts) == M * N)> * = nullptr>
    Mat(const Ts &... components) {
        std::array<T, M * N> arr{((T)components)...};
        std::copy(arr.begin(), arr.end(), &data[0][0]);
    }

    // Mat<2, 2, T>(v) =>
    // [ v, 0 ]
    // [ 0, v ]
    explicit Mat(T component) {
        for (size_t i = 0; i < (M > N ? M : N); i++)
            data[i][i] = component;
    }

    Col getCol(size_t j) const {
        Col col;
        for (size_t i = 0; i < M; i++)
            col[i] = data[i][j];
        return col;
    }

    Row getRow(size_t i) const {
        Row row;
        for (size_t j = 0; j < N; j++)
            row[j] = data[i][j];
        return row;
    }

    const Col &operator[](size_t i) const {
        if (i >= M)
            throw std::out_of_range("Index is larger than Vec column");
        return data[i];
    }

    Col &operator[](size_t i) {
        if (i >= M)
            throw std::out_of_range("Index is larger than Vec column");
        return data[i];
    }

    Mat inverse() const {
        Mat<M, N, T> out;
        for (size_t i = 0; i < M; i++) {
            for (size_t j = 0; j < N; j++) {
                out[i][j] = *this[j][i];
            }
        }
        return out;
    }

    friend std::ostream &operator<<(std::ostream &out, const Mat &m)     {
        for (size_t i = 0; i < M; i++) {
            out << m.getRow(i);
            if (i < M - 1)
                out << "\n";
        }
        return out;
    }
};

// Functions

template <size_t M, size_t N, size_t MN, typename T0, typename T1>
Mat<M, N, decltype(T0(0) * T1(0))> matmul(const Mat<M, MN, T0> &a, const Mat<MN, N, T1> &b) {
    Mat<M, N, decltype(T0(0) * T1(0))> out;
    for (size_t i = 0; i < N; i++)
        for (size_t j = 0; j < M; j++)
            out[i][j] = dot(a.getRow(i), b.getCol(j));
    return out;
}

template <size_t M, size_t N, typename T0, typename T1>
Vec<M, decltype(T0(0), T1(0))> matmul(const Mat<M, N, T0> &a, const Vec<N, T1> &b) {
    Vec<M, decltype(T0(0) * T1(0))> out;
    for (size_t i = 0; i < M; i++)
        out[i] = dot(a.getRow(i), b);
    return out;
}

template <size_t M, size_t N, typename T0, typename T1>
Vec<N, decltype(T0(0), T1(0))> matmul(const Vec<M, T0> &a, const Mat<M, N, T1> &b) {
    Vec<N, decltype(T0(0) * T1(0))> out;
    for (size_t j = 0; j < N; j++)
        out[j] = dot(a, b.getCol(j));
    return out;
}

// Operators

#define ESEED_MAT_PRE(op)                        \
    template <size_t M, size_t N, typename T>    \
    Mat<M, N, T> &operator op(Mat<M, N, T> &m) { \
        for (size_t i = 0; i < M; i++)           \
            op m[i];                             \
        return m;                                \
    }

#define ESEED_MAT_POST(op)                           \
    template <size_t M, size_t N, typename T>        \
    Mat<M, N, T> operator op(Mat<M, N, T> &m, int) { \
        Mat<M, N, T> out = m;                        \
        for (size_t i = 0; i < M; i++)               \
            m[i] op;                                 \
        return out;                                  \
    }

#define ESEED_MAT_UN(op)                              \
    template <size_t M, size_t N, typename T>         \
    Mat<M, N, T> operator op(const Mat<M, N, T> &m) { \
        Mat<M, N, T> out;                             \
        for (size_t i = 0; i < M; i++)                \
            op m[i];                                  \
        return out;                                   \
    }

#define ESEED_MAT_BIN_MM(op)                                                                          \
    template <size_t M, size_t N, typename T0, typename T1>                                           \
    Mat<M, N, decltype(T0(0) op T1(0))> operator op(const Mat<M, N, T0> &a, const Mat<M, N, T1> &b) { \
        Mat<M, N, decltype(T0(0) op T1(0))> out;                                                      \
        for (size_t i = 0; i < M; i++)                                                                \
            out[i] = a[i] op b[i];                                                                    \
        return out;                                                                                   \
    }

#define ESEED_MAT_BIN_MS(op)                                                        \
    template <size_t M, size_t N, typename T0, typename T1>                         \
    Mat<M, N, decltype(T0(0) op T1(0))> operator op(const Mat<M, N, T0> &a, T1 b) { \
        Mat<M, N, decltype(T0(0) op T1(0))> out;                                    \
        for (size_t i = 0; i < M; i++)                                              \
            out[i] = a[i] op b;                                                     \
        return out;                                                                 \
    }

#define ESEED_MAT_BIN_SM(op)                                                               \
    template <size_t M, size_t N, typename T0, typename T1>                                \
    Mat<M, N, decltype(T0(0) op T1(0))> operator op(const T0 &a, const Mat<M, N, T1> &b) { \
        Mat<M, N, decltype(T0(0) op T1(0))> out;                                           \
        for (size_t i = 0; i < M; i++)                                                     \
            out[i] = a op b[i];                                                            \
        return out;                                                                        \
    }

#define ESEED_MAT_ASSN_MM(op)                                                                    \
    template <size_t M, size_t N, typename T0, typename T1>                                      \
    Mat<M, N, decltype(T0(0) op T1(0))> &operator op(Mat<M, N, T0> &a, const Mat<M, N, T1> &b) { \
        for (size_t i = 0; i < M; i++)                                                           \
            a[i] op b[i];                                                                        \
        return a;                                                                                \
    }

#define ESEED_MAT_ASSN_MS(op)                                                  \
    template <size_t M, size_t N, typename T0, typename T1>                    \
    Mat<M, N, decltype(T0(0) op T1(0))> &operator op(Mat<M, N, T0> &a, T1 b) { \
        for (size_t i = 0; i < M; i++)                                         \
            a[i] op b;                                                         \
        return a;                                                              \
    }

ESEED_MAT_PRE(++)
ESEED_MAT_PRE(--)

ESEED_MAT_POST(--)
ESEED_MAT_POST(++)

ESEED_MAT_UN(!)
ESEED_MAT_UN(~)
ESEED_MAT_UN(-)
ESEED_MAT_UN(+)

ESEED_MAT_BIN_MM(+)
ESEED_MAT_BIN_MM(-)
ESEED_MAT_BIN_MM(*)
ESEED_MAT_BIN_MM(/)
ESEED_MAT_BIN_MM(%)
ESEED_MAT_BIN_MM(&)
ESEED_MAT_BIN_MM(|)
ESEED_MAT_BIN_MM (^)
ESEED_MAT_BIN_MM(<<)
ESEED_MAT_BIN_MM(>>)
ESEED_MAT_BIN_MM(&&)
ESEED_MAT_BIN_MM(||)

ESEED_MAT_BIN_MS(+)
ESEED_MAT_BIN_MS(-)
ESEED_MAT_BIN_MS(*)
ESEED_MAT_BIN_MS(/)
ESEED_MAT_BIN_MS(%)
ESEED_MAT_BIN_MS(&)
ESEED_MAT_BIN_MS(|)
ESEED_MAT_BIN_MS (^)
ESEED_MAT_BIN_MS(<<)
ESEED_MAT_BIN_MS(>>)
ESEED_MAT_BIN_MS(&&)
ESEED_MAT_BIN_MS(||)

ESEED_MAT_BIN_SM(+)
ESEED_MAT_BIN_SM(-)
ESEED_MAT_BIN_SM(*)
ESEED_MAT_BIN_SM(/)
ESEED_MAT_BIN_SM(%)
ESEED_MAT_BIN_SM(&)
ESEED_MAT_BIN_SM(|)
ESEED_MAT_BIN_SM (^)
ESEED_MAT_BIN_SM(<<)
ESEED_MAT_BIN_SM(>>)
ESEED_MAT_BIN_SM(&&)
ESEED_MAT_BIN_SM(||)

ESEED_MAT_ASSN_MM(+=)
ESEED_MAT_ASSN_MM(-=)
ESEED_MAT_ASSN_MM(*=)
ESEED_MAT_ASSN_MM(/=)
ESEED_MAT_ASSN_MM(%=)
ESEED_MAT_ASSN_MM(&=)
ESEED_MAT_ASSN_MM(|=)
ESEED_MAT_ASSN_MM(^=)
ESEED_MAT_ASSN_MM(<<=)
ESEED_MAT_ASSN_MM(>>=)

ESEED_MAT_ASSN_MS(+=)
ESEED_MAT_ASSN_MS(-=)
ESEED_MAT_ASSN_MS(*=)
ESEED_MAT_ASSN_MS(/=)
ESEED_MAT_ASSN_MS(%=)
ESEED_MAT_ASSN_MS(&=)
ESEED_MAT_ASSN_MS(|=)
ESEED_MAT_ASSN_MS(^=)
ESEED_MAT_ASSN_MS(<<=)
ESEED_MAT_ASSN_MS(>>=)

}