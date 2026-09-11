#include <chrono>
#include <stdexcept>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "atium/cashflow.hpp"

constexpr double relative_tolerance{1e-12};

using namespace std::chrono;

TEST_CASE("fixed_cashflows creates one cashflow per adjacent period") {
    const std::vector<year_month_day> schedule{2026y / 1 / 2, 2026y / 4 / 2, 2026y / 7 / 2};
    const std::vector<double> year_fractions{0.25, 0.25};

    const std::vector<atium::Cashflow> cashflows{
        atium::fixed_cashflows(schedule, 1'000'000.0, 0.04, year_fractions)};

    REQUIRE(cashflows.size() == 2);
    CHECK(cashflows[0].payment_date == 2026y / 4 / 2);
    CHECK_THAT(cashflows[0].amount, Catch::Matchers::WithinRel(10'000.0, relative_tolerance));
    CHECK(cashflows[1].payment_date == 2026y / 7 / 2);
    CHECK_THAT(cashflows[1].amount, Catch::Matchers::WithinRel(10'000.0, relative_tolerance));
}

TEST_CASE("fixed_cashflows preserves a negative fixed rate") {
    const std::vector<year_month_day> schedule{2026y / 1 / 2, 2026y / 7 / 2};
    const std::vector<double> year_fractions{0.5};

    const std::vector<atium::Cashflow> cashflows{
        atium::fixed_cashflows(schedule, 1'000'000.0, -0.01, year_fractions)};

    REQUIRE(cashflows.size() == 1);
    CHECK(cashflows[0].payment_date == 2026y / 7 / 2);
    CHECK_THAT(cashflows[0].amount, Catch::Matchers::WithinRel(-5'000.0, relative_tolerance));
}

TEST_CASE("fixed_cashflows rejects invalid schedule structure") {
    CHECK_THROWS_AS(atium::fixed_cashflows({2026y / 1 / 2}, 1'000'000.0, 0.04, {}),
                    std::runtime_error);
    CHECK_THROWS_AS(
        atium::fixed_cashflows({2026y / 1 / 2, 2026y / 2 / 29}, 1'000'000.0, 0.04, {0.25}),
        std::runtime_error);
    CHECK_THROWS_AS(
        atium::fixed_cashflows({2026y / 1 / 2, 2026y / 1 / 2}, 1'000'000.0, 0.04, {0.25}),
        std::runtime_error);
    CHECK_THROWS_AS(
        atium::fixed_cashflows({2026y / 1 / 2, 2026y / 4 / 2}, 1'000'000.0, 0.04, {0.25, 0.25}),
        std::runtime_error);
}
