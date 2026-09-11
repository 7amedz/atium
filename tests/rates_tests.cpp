#include <limits>
#include <stdexcept>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "atium/rates.hpp"

constexpr double relative_tolerance = 1e-12;

TEST_CASE("simple_discount_factor discounts a positive rate") {
    const double rate = 0.05;
    const double year_fraction = 0.5;
    const double expected = 1.0 / 1.025;

    CHECK_THAT(atium::simple_discount_factor(rate, year_fraction),
               Catch::Matchers::WithinRel(expected, relative_tolerance));
}

TEST_CASE("simple_discount_factor accepts a negative rate") {
    CHECK_THAT(atium::simple_discount_factor(-0.01, 0.5),
               Catch::Matchers::WithinRel(1.0 / 0.995, relative_tolerance));
}

TEST_CASE("simple_discount_factor rejects invalid inputs") {
    CHECK_THROWS_AS(atium::simple_discount_factor(0.05, 0.0), std::runtime_error);
    CHECK_THROWS_AS(atium::simple_discount_factor(0.05, -0.5), std::runtime_error);
    CHECK_THROWS_AS(atium::simple_discount_factor(std::numeric_limits<double>::quiet_NaN(), 0.5),
                    std::runtime_error);
    CHECK_THROWS_AS(atium::simple_discount_factor(0.05, std::numeric_limits<double>::infinity()),
                    std::runtime_error);
    CHECK_THROWS_AS(atium::simple_discount_factor(-2.0, 0.5), std::runtime_error);
}

TEST_CASE("simple_forward_rate derives a positive rate from factors") {
    const double start_factor = 1.0;
    const double end_factor = 0.99;
    const double year_fraction = 0.25;
    const double expected = ((1.0 / 0.99) - 1.0) / 0.25;

    CHECK_THAT(atium::simple_forward_rate(start_factor, end_factor, year_fraction),
               Catch::Matchers::WithinRel(expected, relative_tolerance));
}

TEST_CASE("simple_forward_rate accepts a negative rate") {
    CHECK_THAT(atium::simple_forward_rate(0.99, 1.0, 0.25),
               Catch::Matchers::WithinRel(-0.04, relative_tolerance));
}

TEST_CASE("simple_forward_rate rejects invalid inputs") {
    CHECK_THROWS_AS(atium::simple_forward_rate(0.0, 0.99, 0.25), std::runtime_error);
    CHECK_THROWS_AS(atium::simple_forward_rate(1.0, -0.99, 0.25), std::runtime_error);
    CHECK_THROWS_AS(
        atium::simple_forward_rate(std::numeric_limits<double>::quiet_NaN(), 0.99, 0.25),
        std::runtime_error);
    CHECK_THROWS_AS(atium::simple_forward_rate(1.0, std::numeric_limits<double>::infinity(), 0.25),
                    std::runtime_error);
    CHECK_THROWS_AS(atium::simple_forward_rate(1.0, 0.99, 0.0), std::runtime_error);
}
