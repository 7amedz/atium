#include <chrono>
#include <stdexcept>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "atium/cashflow.hpp"

constexpr double relative_tolerance{1e-12};

using namespace std::chrono;

TEST_CASE("leg_present_value discounts and receives a leg") {
    const std::vector<atium::Cashflow> cashflows{
        {.payment_date = 2026y / 4 / 2, .amount = 10'000.0},
        {.payment_date = 2026y / 7 / 2, .amount = 10'000.0},
    };
    const std::vector<double> discount_factors{0.99, 0.97};

    CHECK_THAT(atium::leg_present_value(cashflows, discount_factors, atium::PayReceive::Receive),
               Catch::Matchers::WithinRel(19'600.0, relative_tolerance));
}

TEST_CASE("leg_present_value applies pay direction to a signed leg") {
    const std::vector<atium::Cashflow> cashflows{
        {.payment_date = 2026y / 7 / 2, .amount = -5'000.0},
    };
    const std::vector<double> discount_factors{0.98};

    CHECK_THAT(atium::leg_present_value(cashflows, discount_factors, atium::PayReceive::Receive),
               Catch::Matchers::WithinRel(-4'900.0, relative_tolerance));
    CHECK_THAT(atium::leg_present_value(cashflows, discount_factors, atium::PayReceive::Pay),
               Catch::Matchers::WithinRel(4'900.0, relative_tolerance));
}

TEST_CASE("leg_present_value accepts an empty leg") {
    CHECK(atium::leg_present_value({}, {}, atium::PayReceive::Receive) == 0.0);
    CHECK(atium::leg_present_value({}, {}, atium::PayReceive::Pay) == 0.0);
}

TEST_CASE("leg_present_value rejects invalid inputs") {
    const std::vector<atium::Cashflow> cashflows{
        {.payment_date = 2026y / 4 / 2, .amount = 10'000.0},
    };

    CHECK_THROWS_AS(atium::leg_present_value(cashflows, {}, atium::PayReceive::Receive),
                    std::runtime_error);
    CHECK_THROWS_AS(atium::leg_present_value(cashflows, {0.0}, atium::PayReceive::Receive),
                    std::runtime_error);
    CHECK_THROWS_AS(atium::leg_present_value({}, {}, static_cast<atium::PayReceive>(42)),
                    std::runtime_error);
}
