#include <chrono>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#include "atium/schedule.hpp"

using namespace std::chrono;

// using months	= duration<_GLIBCXX_CHRONO_INT64_T, ratio<2629746>>; from chrono
// we use DurationM for freq

TEST_CASE("regular annual, nothing capped") {
    const year_month_day start_date{2026y / 9 / 7};
    const year_month_day termination_date{2029y / 9 / 7};
    const atium::DurationM frequency{12};

    CHECK(start_date.ok());
    CHECK(termination_date.ok());

    std::vector<year_month_day> expected{2026y / 9 / 7, 2027y / 9 / 7, 2028y / 9 / 7,
                                         2029y / 9 / 7};
    REQUIRE(atium::generate_schedule(start_date, termination_date, frequency) == expected);
}

TEST_CASE("regular semi-annual, five years") {
    const year_month_day start_date{2026y / 9 / 7};
    const year_month_day termination_date{2031y / 9 / 7};
    const atium::DurationM frequency{6};

    CHECK(start_date.ok());
    CHECK(termination_date.ok());

    std::vector<year_month_day> expected{2026y / 9 / 7, 2027y / 3 / 7, 2027y / 9 / 7, 2028y / 3 / 7,
                                         2028y / 9 / 7, 2029y / 3 / 7, 2029y / 9 / 7, 2030y / 3 / 7,
                                         2030y / 9 / 7, 2031y / 3 / 7, 2031y / 9 / 7};

    REQUIRE(atium::generate_schedule(start_date, termination_date, frequency) == expected);
}

TEST_CASE("regular quarterly, one year") {
    const year_month_day start_date{2026y / 9 / 7};
    const year_month_day termination_date{2027y / 9 / 7};
    const atium::DurationM frequency{3};

    CHECK(start_date.ok());
    CHECK(termination_date.ok());

    std::vector<year_month_day> expected{2026y / 9 / 7, 2026y / 12 / 7, 2027y / 3 / 7,
                                         2027y / 6 / 7, 2027y / 9 / 7};

    REQUIRE(atium::generate_schedule(start_date, termination_date, frequency) == expected);
}

TEST_CASE("ten years, ten periods") {
    const year_month_day start_date{2026y / 9 / 7};
    const year_month_day termination_date{2036y / 9 / 7};
    const atium::DurationM frequency{12};

    CHECK(start_date.ok());
    CHECK(termination_date.ok());

    std::vector<year_month_day> expected{2026y / 9 / 7, 2027y / 9 / 7, 2028y / 9 / 7, 2029y / 9 / 7,
                                         2030y / 9 / 7, 2031y / 9 / 7, 2032y / 9 / 7, 2033y / 9 / 7,
                                         2034y / 9 / 7, 2035y / 9 / 7, 2036y / 9 / 7};

    REQUIRE(atium::generate_schedule(start_date, termination_date, frequency) == expected);
}

TEST_CASE("roll day 31: four caps, two recoveries to 31") {
    const year_month_day start_date{2026y / 8 / 31};
    const year_month_day termination_date{2028y / 8 / 31};
    const atium::DurationM frequency{3};

    CHECK(start_date.ok());
    CHECK(termination_date.ok());

    std::vector<year_month_day> expected{2026y / 8 / 31, 2026y / 11 / 30, 2027y / 2 / 28,
                                         2027y / 5 / 31, 2027y / 8 / 31,  2027y / 11 / 30,
                                         2028y / 2 / 29, 2028y / 5 / 31,  2028y / 8 / 31};

    REQUIRE(atium::generate_schedule(start_date, termination_date, frequency) == expected);
}

TEST_CASE("31st through February") {
    const year_month_day start_date{2026y / 1 / 31};
    const year_month_day termination_date{2026y / 7 / 31};
    const atium::DurationM frequency{3};

    CHECK(start_date.ok());
    CHECK(termination_date.ok());

    std::vector<year_month_day> expected{2026y / 1 / 31, 2026y / 4 / 30, 2026y / 7 / 31};

    REQUIRE(atium::generate_schedule(start_date, termination_date, frequency) == expected);
}

TEST_CASE("anchor is a 28-day month end") {
    const year_month_day start_date{2026y / 11 / 30};
    const year_month_day termination_date{2027y / 2 / 28};
    const atium::DurationM frequency{3};

    CHECK(start_date.ok());
    CHECK(termination_date.ok());

    std::vector<year_month_day> expected{2026y / 11 / 30, 2027y / 2 / 28};

    REQUIRE(atium::generate_schedule(start_date, termination_date, frequency) == expected);
}

TEST_CASE("leap-day start") {
    const year_month_day start_date{2028y / 2 / 29};
    const year_month_day termination_date{2029y / 2 / 28};
    const atium::DurationM frequency{12};

    CHECK(start_date.ok());
    CHECK(termination_date.ok());

    std::vector<year_month_day> expected{2028y / 2 / 29, 2029y / 2 / 28};

    REQUIRE(atium::generate_schedule(start_date, termination_date, frequency) == expected);
}

TEST_CASE("February cap recovers to roll day 30; end-of-month is false") {
    const year_month_day start_date{2026y / 2 / 28};
    const year_month_day termination_date{2026y / 8 / 30};
    const atium::DurationM frequency{3};

    CHECK(start_date.ok());
    CHECK(termination_date.ok());

    std::vector<year_month_day> expected{2026y / 2 / 28, 2026y / 5 / 30, 2026y / 8 / 30};

    REQUIRE(atium::generate_schedule(start_date, termination_date, frequency) == expected);
}

TEST_CASE("artefact: walk hits 2026-08-28, short first period") {
    const year_month_day start_date{2026y / 8 / 31};
    const year_month_day termination_date{2027y / 2 / 28};
    const atium::DurationM frequency{3};

    CHECK(start_date.ok());
    CHECK(termination_date.ok());

    std::vector<year_month_day> expected{2026y / 8 / 31, 2026y / 11 / 28, 2027y / 2 / 28};

    REQUIRE(atium::generate_schedule(start_date, termination_date, frequency) == expected);
}

TEST_CASE("artefact: walk hits 2026-03-30, short first period") {
    const year_month_day start_date{2026y / 3 / 31};
    const year_month_day termination_date{2026y / 9 / 30};
    const atium::DurationM frequency{3};

    CHECK(start_date.ok());
    CHECK(termination_date.ok());

    std::vector<year_month_day> expected{2026y / 3 / 31, 2026y / 6 / 30, 2026y / 9 / 30};

    REQUIRE(atium::generate_schedule(start_date, termination_date, frequency) == expected);
}

TEST_CASE("two weeks under a quarterly frequency") {
    const year_month_day start_date{2026y / 9 / 7};
    const year_month_day termination_date{2026y / 9 / 21};
    const atium::DurationM frequency{3};

    CHECK(start_date.ok());
    CHECK(termination_date.ok());

    std::vector<year_month_day> expected{2026y / 9 / 7, 2026y / 9 / 21};

    REQUIRE(atium::generate_schedule(start_date, termination_date, frequency) == expected);
}

TEST_CASE("one month under a quarterly frequency") {
    const year_month_day start_date{2026y / 9 / 7};
    const year_month_day termination_date{2026y / 10 / 7};
    const atium::DurationM frequency{3};

    CHECK(start_date.ok());
    CHECK(termination_date.ok());

    std::vector<year_month_day> expected{2026y / 9 / 7, 2026y / 10 / 7};

    REQUIRE(atium::generate_schedule(start_date, termination_date, frequency) == expected);
}

TEST_CASE("one month under an annual frequency") {
    const year_month_day start_date{2026y / 9 / 7};
    const year_month_day termination_date{2026y / 10 / 7};
    const atium::DurationM frequency{12};

    CHECK(start_date.ok());
    CHECK(termination_date.ok());

    std::vector<year_month_day> expected{2026y / 9 / 7, 2026y / 10 / 7};

    REQUIRE(atium::generate_schedule(start_date, termination_date, frequency) == expected);
}

TEST_CASE("one quarter under an annual frequency") {
    const year_month_day start_date{2026y / 9 / 7};
    const year_month_day termination_date{2026y / 12 / 7};
    const atium::DurationM frequency{12};

    CHECK(start_date.ok());
    CHECK(termination_date.ok());

    std::vector<year_month_day> expected{2026y / 9 / 7, 2026y / 12 / 7};

    REQUIRE(atium::generate_schedule(start_date, termination_date, frequency) == expected);
}

TEST_CASE("six months under an annual frequency") {
    const year_month_day start_date{2026y / 9 / 7};
    const year_month_day termination_date{2027y / 3 / 7};
    const atium::DurationM frequency{12};

    CHECK(start_date.ok());
    CHECK(termination_date.ok());

    std::vector<year_month_day> expected{2026y / 9 / 7, 2027y / 3 / 7};

    REQUIRE(atium::generate_schedule(start_date, termination_date, frequency) == expected);
}

TEST_CASE("exactly one period") {
    const year_month_day start_date{2026y / 9 / 7};
    const year_month_day termination_date{2027y / 9 / 7};
    const atium::DurationM frequency{12};

    CHECK(start_date.ok());
    CHECK(termination_date.ok());

    std::vector<year_month_day> expected{2026y / 9 / 7, 2027y / 9 / 7};

    REQUIRE(atium::generate_schedule(start_date, termination_date, frequency) == expected);
}
