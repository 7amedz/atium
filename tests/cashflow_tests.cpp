
#include <limits>
#include <stdexcept>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "atium/cashflow.hpp"

constexpr double relative_tolerance = 1e-12;

using namespace std::chrono;

TEST_CASE("present_value discounts a positive cashflow") {

    const atium::Cashflow cf{.payment_date = 2025y / 5 / 5, .amount = 10000.0};
    const double discount_factor = 0.985;
    CHECK_THAT(atium::present_value(cf, discount_factor),
               Catch::Matchers::WithinRel(9850.0, relative_tolerance));
}

TEST_CASE("present_value preserves a negative cashflow sign") {

    const atium::Cashflow cf{.payment_date = 2025y / 5 / 5, .amount = -10000.0};
    const double discount_factor = 0.985;
    CHECK_THAT(atium::present_value(cf, discount_factor),
               Catch::Matchers::WithinRel(-9850.0, relative_tolerance));
}

TEST_CASE("present_value accepts the unit factor") {

    const atium::Cashflow cf{.payment_date = 2025y / 5 / 5, .amount = 10000.0};
    CHECK_THAT(atium::present_value(cf, 1.0),
               Catch::Matchers::WithinRel(10000.0, relative_tolerance));
}

TEST_CASE("present_value accepts a factor above one") {

    const atium::Cashflow cf{.payment_date = 2025y / 5 / 5, .amount = 10000.0};
    CHECK_THAT(atium::present_value(cf, 1.005),
               Catch::Matchers::WithinRel(10050.0, relative_tolerance));
}

TEST_CASE("present_value rejects invalid discount factors") {

    const atium::Cashflow cf{.payment_date = 2025y / 5 / 5, .amount = 10000.0};

    CHECK_THROWS_AS(atium::present_value(cf, 0.0), std::runtime_error);
    CHECK_THROWS_AS(atium::present_value(cf, -0.985), std::runtime_error);
    CHECK_THROWS_AS(atium::present_value(cf, std::numeric_limits<double>::infinity()),
                    std::runtime_error);
    CHECK_THROWS_AS(atium::present_value(cf, std::numeric_limits<double>::quiet_NaN()),
                    std::runtime_error);
}
