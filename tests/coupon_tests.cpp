#include <limits>
#include <stdexcept>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "atium/coupon.hpp"

constexpr double relative_tolerance = 1e-12;

TEST_CASE("coupon_amount calculates a positive coupon") {
    const double notional = 1'000'000.0;
    const double rate = 0.05;
    const double year_fraction = 0.5;
    const double expected = 25'000.0;

    CHECK_THAT(atium::coupon_amount(notional, rate, year_fraction),
               Catch::Matchers::WithinRel(expected, relative_tolerance));
}

TEST_CASE("coupon_amount accepts a negative rate") {
    CHECK_THAT(atium::coupon_amount(1'000'000.0, -0.01, 0.5),
               Catch::Matchers::WithinRel(-5'000.0, relative_tolerance));
}

TEST_CASE("coupon_amount rejects invalid inputs") {
    CHECK_THROWS_AS(atium::coupon_amount(0.0, 0.05, 0.5), std::runtime_error);
    CHECK_THROWS_AS(atium::coupon_amount(-1'000'000.0, 0.05, 0.5), std::runtime_error);
    CHECK_THROWS_AS(
        atium::coupon_amount(1'000'000.0, std::numeric_limits<double>::quiet_NaN(), 0.5),
        std::runtime_error);
    CHECK_THROWS_AS(
        atium::coupon_amount(1'000'000.0, 0.05, std::numeric_limits<double>::infinity()),
        std::runtime_error);
    CHECK_THROWS_AS(atium::coupon_amount(std::numeric_limits<double>::max(), 2.0, 1.0),
                    std::runtime_error);
}
