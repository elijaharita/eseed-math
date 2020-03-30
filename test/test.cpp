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

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <eseed/math/ops.hpp>
#include <eseed/math/vec.hpp>
#include <eseed/math/vecops.hpp>
#include <eseed/math/matops.hpp>
#include <iostream>

TEST_CASE("scalar functions", "[scalar]") {

    SECTION("special floating point values") {
        REQUIRE(esdm::inf<float>() == std::numeric_limits<float>::infinity());
        REQUIRE(esdm::isinf(std::numeric_limits<float>::infinity()));
        REQUIRE(esdm::isinf(-std::numeric_limits<float>::infinity()));
        REQUIRE(!esdm::isinf(5.f));
        // No way to check for quiet or signaling NaN that I know of =(
        // Hopefully it just works I guess
        REQUIRE(esdm::nan<float>() != esdm::nan<float>());
        REQUIRE(esdm::isnan<float>(NAN));
    }

    // Constants (pi, etc.) are correct

    SECTION("general functions") {
        REQUIRE(esdm::abs(-5) == 5);
        REQUIRE(esdm::sq(5) == 25);
        REQUIRE(esdm::sqrt(25) == 5);
        REQUIRE(esdm::pow(2, 4) == 16);
    }
    
    SECTION("rounding") {
        for (float f = -1.f; f <= 1.f; f += 0.25f) {
            REQUIRE(esdm::trunc(f) == std::trunc(f));
            REQUIRE(esdm::round(f) == std::round(f));
            REQUIRE(esdm::floor(f) == std::floor(f));
            REQUIRE(esdm::ceil(f) == std::ceil(f));
        }
    }

    SECTION("direct-to-int rounding") {
        for (float f = -1.f; f <= 1.f; f += 0.25f) {
            REQUIRE(esdm::itrunc<int>(f) == (int)std::trunc(f));
            REQUIRE(esdm::iround<int>(f) == (int)std::round(f));
            REQUIRE(esdm::ifloor<int>(f) == (int)std::floor(f));
            REQUIRE(esdm::iceil<int>(f) == (int)std::ceil(f));
        }
    }

    // At this point trigonometry functions are guaranteed correct because
    // they're just wrappers over standard library functions
}

TEST_CASE("vector constructors", "[vector]") {
    SECTION("default") {
        constexpr esdm::Vec3<float> v;

        REQUIRE(v.getX() == 0.f);
        REQUIRE(v.getY() == 0.f);
        REQUIRE(v.getZ() == 0.f);
    }

    SECTION("multi element") {
        constexpr esdm::Vec3<float> v(1, 2, 3);

        REQUIRE(v.getX() == 1.f);
        REQUIRE(v.getY() == 2.f);
        REQUIRE(v.getZ() == 3.f);
    }

    SECTION("multi element incomplete") {
        constexpr esdm::Vec3<float> v(1, 2);

        REQUIRE(v.getX() == 1.f);
        REQUIRE(v.getY() == 2.f);
        REQUIRE(v.getZ() == 0.f);
    }

    SECTION("type / length conversion") {
        constexpr esdm::Vec2<float> a(1, 2);
        constexpr esdm::Vec3<int> b(a);

        REQUIRE(b.getX() == 1);
        REQUIRE(b.getY() == 2);
        REQUIRE(b.getZ() == 0);
    }
}

TEST_CASE("vector operators", "[vector]") {
    SECTION("comparison") {
        constexpr esdm::Vec3<float> a(1, 2, 3);
        constexpr esdm::Vec3<float> b(1, 2, 3);
        REQUIRE(a == b);
    }

    SECTION("unary") {
        esdm::Vec3<float> v = { 1.f, 2.f, 3.f };

        REQUIRE(-v == esdm::Vec3<float>(-1.f, -2.f, -3.f));
    }

    SECTION("binary") {
        constexpr esdm::Vec3<float> a = { 1.f, 2.f, 3.f };
        constexpr esdm::Vec3<float> b = { 4.f, 5.f, 6.f };
        constexpr esdm::Vec3<float> vv = a + b;

        REQUIRE(a + b == esdm::Vec3<float>(5.f, 7.f, 9.f));
        REQUIRE(a + 1.f == esdm::Vec3<float>(2.f, 3.f, 4.f));
        REQUIRE(1.f + a == esdm::Vec3<float>(2.f, 3.f, 4.f));
    }

    SECTION("assignment") {
        esdm::Vec3<float> a = { 1.f, 2.f, 3.f };
        esdm::Vec3<float> b = { 4.f, 5.f, 6.f };

        REQUIRE((a += b) == esdm::Vec3<float>(5.f, 7.f, 9.f));
        REQUIRE(a == esdm::Vec3<float>(5.f, 7.f, 9.f));
        REQUIRE((a += 1.f) == esdm::Vec3<float>(6.f, 8.f, 10.f));
        REQUIRE(a == esdm::Vec3<float>(6.f, 8.f, 10.f));
    }

    SECTION("subscript") {
        esdm::Vec3<float> a;
        a[0] = 1.f;
        REQUIRE(a[0] == 1.f);
        
        constexpr esdm::Vec3<float> b(1.f, 2.f, 3.f);
        REQUIRE(b[0] == 1.f);
    }
}

TEST_CASE("special vector accessors", "[vector]") {
    SECTION("accessors") {
        esdm::Vec4<float> v(1, 2, 3, 4);

        REQUIRE(v.x() == 1);
        REQUIRE(v.y() == 2);
        REQUIRE(v.z() == 3);
        REQUIRE(v.w() == 4);
    }

    SECTION("getters") {
        constexpr esdm::Vec4<float> v(1, 2, 3, 4);

        REQUIRE(v.getX() == 1);
        REQUIRE(v.getY() == 2);
        REQUIRE(v.getZ() == 3);
        REQUIRE(v.getW() == 4);
    }

    SECTION("getters") {
        esdm::Vec4<float> v;
        
        v.setX(1);
        v.setY(2);
        v.setZ(3);
        v.setW(4);
    }
}

TEST_CASE("vector functions", "[vector]") {
    SECTION("abs") {
        REQUIRE(esdm::abs(esdm::Vec3<float>(-1.f, 2.f, -3.f)) == esdm::Vec3<float>(1.f, 2.f, 3.f));
    }

    SECTION("rounding") {
        REQUIRE(
            esdm::round(esdm::Vec3<float>(0.25f, 0.5f, 0.75f)) == 
            esdm::Vec3<float>(
                std::round(0.25f),
                std::round(0.5f),
                std::round(0.75f)
            )
        );
        REQUIRE(
            esdm::trunc(esdm::Vec3<float>(0.5f, 1.5f, -0.5f)) == 
            esdm::Vec3<float>(
                std::trunc(0.5f),
                std::trunc(1.5f),
                std::trunc(-0.5f)
            )
        );
        REQUIRE(
            esdm::floor(esdm::Vec3<float>(0.5f, 1.5f, -0.5f)) == 
            esdm::Vec3<float>(
                std::floor(0.5f),
                std::floor(1.5f),
                std::floor(-0.5f)
            )
        );
        REQUIRE(
            esdm::ceil(esdm::Vec3<float>(0.5f, 1.5f, -0.5f)) == 
            esdm::Vec3<float>(
                std::ceil(0.5f),
                std::ceil(1.5f),
                std::ceil(-0.5f)
            )
        );
    }

    SECTION("direct-to-int rounding") {
        REQUIRE(
            esdm::iround<int>(esdm::Vec3<float>(0.25f, 0.5f, 0.75f)) == 
            esdm::Vec3<int>(
                std::round(0.25f),
                std::round(0.5f),
                std::round(0.75f)
            )
        );
        REQUIRE(
            esdm::itrunc<int>(esdm::Vec3<float>(0.5f, 1.5f, -0.5f)) == 
            esdm::Vec3<int>(
                std::trunc(0.5f),
                std::trunc(1.5f),
                std::trunc(-0.5f)
            )
        );
        REQUIRE(
            esdm::ifloor<int>(esdm::Vec3<float>(0.5f, 1.5f, -0.5f)) == 
            esdm::Vec3<int>(
                std::floor(0.5f),
                std::floor(1.5f),
                std::floor(-0.5f)
            )
        );
        REQUIRE(
            esdm::iceil<int>(esdm::Vec3<float>(0.5f, 1.5f, -0.5f)) == 
            esdm::Vec3<int>(
                std::ceil(0.5f),
                std::ceil(1.5f),
                std::ceil(-0.5f)
            )
        );
    }

    SECTION("dot product") {
        constexpr float n = esdm::dot(
            esdm::Vec3<float>(1.f, 2.f, 3.f), 
            esdm::Vec3<float>(4.f, 5.f, 6.f)
        );
        
        REQUIRE(n == 32.f);
    }

    SECTION("cross product") {
        constexpr esdm::Vec3<float> a(2.f, 3.f, 4.f);
        constexpr esdm::Vec3<float> b(5.f, 6.f, 7.f);
        constexpr esdm::Vec3<float> c = esdm::cross(a, b);

        REQUIRE(c == esdm::Vec3<float>(-3.f, 6.f, -3.f));
    }
}

TEST_CASE("matrix constructors", "[matrix]") {
    SECTION("default") {
        constexpr esdm::Mat2<float> m;
        REQUIRE(m[0][0] == 0);
        REQUIRE(m[0][1] == 0);
        REQUIRE(m[1][0] == 0);
        REQUIRE(m[1][1] == 0);
    }

    SECTION("multi element") {
        constexpr esdm::Mat2<float> m(
            1, 2, 
            3, 4
        );
        REQUIRE(m[0][0] == 1);
        REQUIRE(m[0][1] == 2);
        REQUIRE(m[1][0] == 3);
        REQUIRE(m[1][1] == 4);
    }

    SECTION("type / size conversion") {
        constexpr esdm::Mat3<float> a(
            1, 2, 3, 
            4, 5, 6, 
            7, 8, 9
        );
        constexpr esdm::Mat2<int> b(a);

        REQUIRE(b[0][0] == 1);
        REQUIRE(b[0][1] == 2);
        REQUIRE(b[1][0] == 4);
        REQUIRE(b[1][1] == 5);
    }

    SECTION("type conversion only") {
        constexpr esdm::Mat2<float> a(
            1, 2,
            3, 4
        );
        constexpr esdm::Mat2<int> b(a);
        
        REQUIRE(b[0][0] == 1);
        REQUIRE(b[0][1] == 2);
        REQUIRE(b[1][0] == 3);
        REQUIRE(b[1][1] == 4);
    }

    SECTION("identity") {
        auto m = esdm::Mat2<float>::ident();
        
        REQUIRE(m[0][0] == 1);
        REQUIRE(m[0][1] == 0);
        REQUIRE(m[1][0] == 0);
        REQUIRE(m[1][1] == 1);
    }
}

TEST_CASE("matrix operators", "[matrix]") {
    SECTION("comparison") {
        constexpr esdm::Mat2<float> a(1, 2, 3, 4);
        constexpr esdm::Mat2<float> b(1, 2, 3, 4);
        REQUIRE(a == b);
    }

    SECTION("unary") {
        constexpr esdm::Mat2<float> v(1, 2, 3, 4);

        REQUIRE(-v == esdm::Mat2<float>(-1, -2, -3, -4));
    }

    SECTION("binary") {
        constexpr esdm::Mat2<float> a(1, 2, 3, 4);
        constexpr esdm::Mat2<float> b(5, 6, 7, 8);

        REQUIRE(a + b == esdm::Mat2<float>(6, 8, 10, 12));
        REQUIRE(a + 1.f == esdm::Mat2<float>(2, 3, 4, 5));
        REQUIRE(1.f + a == esdm::Mat2<float>(2, 3, 4, 5));
    }

    SECTION("assignment") {
        esdm::Mat2<float> a(1, 2, 3, 4);
        esdm::Mat2<float> b(5, 6, 7, 8);

        REQUIRE((a += b) == esdm::Mat2<float>(6, 8, 10, 12));
        REQUIRE(a == esdm::Mat2<float>(6, 8, 10, 12));
        REQUIRE((b += 1.f) == esdm::Mat2<float>(6, 7, 8, 9));
        REQUIRE(b == esdm::Mat2<float>(6, 7, 8, 9));
    }

    SECTION("subscript") {
        esdm::Mat2<float> a;
        a[0][0] = 1.f;
        REQUIRE(a[0][0] == 1.f);
        
        constexpr esdm::Mat2<float> b(1.f, 2.f, 3.f, 4.f);
        REQUIRE(b[0][0] == 1.f);
    }
}

TEST_CASE("matrix accessors", "[matrix]") {
    constexpr esdm::Mat2<float> m(
        1, 2,
        3, 4
    );
    
    SECTION("col") {
        constexpr esdm::Mat2<float>::Col col = m.getCol(0);
        REQUIRE(col == esdm::Vec2<float>(1, 3));
    }

    SECTION("row") {
        constexpr esdm::Mat2<float>::Row row = m.getRow(0);
        REQUIRE(row == esdm::Vec2<float>(1, 2));
    }
}

TEST_CASE("matrix functions", "[matrix]") {
    SECTION("transpose") {
        constexpr esdm::Mat2<float> a(1, 2, 3, 4);
        constexpr esdm::Mat2<float> b = esdm::transpose(a);
        REQUIRE(b == esdm::Mat2<float>(1, 3, 2, 4));
    }

    SECTION("multiplication") {
        constexpr esdm::Mat2<float> a(1, 2, 3, 4);
        constexpr esdm::Mat2<float> b(5, 6, 7, 8);
        constexpr esdm::Mat2<float> c = a * b;
        REQUIRE(c == esdm::Mat2<float>(19, 22, 43, 50));

        constexpr esdm::Vec2<float> v(1, 2);

        constexpr esdm::Vec2<float> d = a * v;
        constexpr esdm::Vec2<float> e = v * a;

        REQUIRE(d == esdm::Vec2<float>(5, 11));
        REQUIRE(e == esdm::Vec2<float>(7, 10));
    }
}