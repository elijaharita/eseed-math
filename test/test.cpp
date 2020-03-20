#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <eseed/math/vec.hpp>
#include <iostream>

TEST_CASE("scalar functions", "[scalar]") {
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
}

TEST_CASE("vector constructors", "[vector]") {
    SECTION("default") {
        esdm::Vec3<float> v;

        REQUIRE(v[0] == 0.f);
        REQUIRE(v[1] == 0.f);
        REQUIRE(v[2] == 0.f);
    }

    SECTION("array") {
        float data[] = { 1.f, 2.f, 3.f };
        esdm::Vec3<float> v(data);

        REQUIRE(v[0] == 1.f);
        REQUIRE(v[1] == 2.f);
        REQUIRE(v[2] == 3.f);
    }

    SECTION("single element") {
        esdm::Vec3<float> v(1.f);

        REQUIRE(v[0] == 1.f);
        REQUIRE(v[1] == 1.f);
        REQUIRE(v[2] == 1.f);
    }

    SECTION("multi element") {
        esdm::Vec3<float> v(1.f, 2.f, 3.f);

        REQUIRE(v[0] == 1.f);
        REQUIRE(v[1] == 2.f);
        REQUIRE(v[2] == 3.f);
    }

    SECTION("initializer list") {
        esdm::Vec3<float> v = { 1.f, 2.f, 3.f };

        REQUIRE(v[0] == 1.f);
        REQUIRE(v[1] == 2.f);
        REQUIRE(v[2] == 3.f);
    }
}

TEST_CASE("vector operators", "[vector]") {
    SECTION("comparison") {
        REQUIRE(esdm::Vec3<float>(1.f, 2.f, 3.f) == esdm::Vec3<float>(1.f, 2.f, 3.f));
    }

    SECTION("unary") {
        esdm::Vec3<float> v = { 1.f, 2.f, 3.f };

        REQUIRE(-v == esdm::Vec3<float>(-1.f, -2.f, -3.f));
    }

    SECTION("binary") {
        esdm::Vec3<float> a = { 1.f, 2.f, 3.f };
        esdm::Vec3<float> b = { 4.f, 5.f, 6.f };

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
        REQUIRE(
            esdm::dot(
                esdm::Vec3<float>(1.f, 2.f, 3.f), 
                esdm::Vec3<float>(4.f, 5.f, 6.f)
            ) == 32.f
        );
    }

    SECTION("cross product") {
        REQUIRE(
            esdm::cross(
                esdm::Vec3<float>(2.f, 3.f, 4.f),
                esdm::Vec3<float>(5.f, 6.f, 7.f)
            ) == esdm::Vec3<float>(-3.f, 6.f, -3.f)
        );
    }
}

TEST_CASE("matrix constructors", "[matrix]") {
    
}