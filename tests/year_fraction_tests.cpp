#include <chrono>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp> //for within abs

#include "atium/year_fraction.hpp"

using namespace std::chrono;

const double tolerance = 1e-12;

TEST_CASE("ordinary quarter") {
    const year_month_day start{2026y / September / 7};
    const year_month_day end{2026y / December / 7};
    CHECK(start.ok());
    CHECK(end.ok());
    CHECK((sys_days{end} - sys_days{start}).count() == 91);

    CHECK_THAT(atium::act_365f(start, end),
               Catch::Matchers::WithinAbs(0.2493150684931507, tolerance));
    CHECK_THAT(atium::act_360(start, end),
               Catch::Matchers::WithinAbs(0.25277777777777777, tolerance));
    CHECK_THAT(atium::thirty_360_bb(start, end), Catch::Matchers::WithinAbs(0.25, tolerance));
    CHECK_THAT(atium::thirty_360_e(start, end), Catch::Matchers::WithinAbs(0.25, tolerance));
}

TEST_CASE("exactly one year; ACT/360 still gives 1.0139") {
    const year_month_day start{2026y / January / 1};
    const year_month_day end{2027y / January / 1};
    CHECK(start.ok());
    CHECK(end.ok());
    CHECK((sys_days{end} - sys_days{start}).count() == 365);

    CHECK_THAT(atium::act_365f(start, end), Catch::Matchers::WithinAbs(1.0, tolerance));
    CHECK_THAT(atium::act_360(start, end),
               Catch::Matchers::WithinAbs(1.0138888888888888, tolerance));
    CHECK_THAT(atium::thirty_360_bb(start, end), Catch::Matchers::WithinAbs(1.0, tolerance));
    CHECK_THAT(atium::thirty_360_e(start, end), Catch::Matchers::WithinAbs(1.0, tolerance));
}

TEST_CASE("366 days, contains a leap day") {
    const year_month_day start{2027y / September / 7};
    const year_month_day end{2028y / September / 7};
    CHECK(start.ok());
    CHECK(end.ok());
    CHECK((sys_days{end} - sys_days{start}).count() == 366);

    CHECK_THAT(atium::act_365f(start, end),
               Catch::Matchers::WithinAbs(1.0027397260273974, tolerance));
    CHECK_THAT(atium::act_360(start, end),
               Catch::Matchers::WithinAbs(1.0166666666666666, tolerance));
    CHECK_THAT(atium::thirty_360_bb(start, end), Catch::Matchers::WithinAbs(1.0, tolerance));
    CHECK_THAT(atium::thirty_360_e(start, end), Catch::Matchers::WithinAbs(1.0, tolerance));
}

TEST_CASE("month end to month end") {
    const year_month_day start{2026y / August / 31};
    const year_month_day end{2026y / September / 30};
    CHECK(start.ok());
    CHECK(end.ok());
    CHECK((sys_days{end} - sys_days{start}).count() == 30);

    CHECK_THAT(atium::act_365f(start, end),
               Catch::Matchers::WithinAbs(0.0821917808219178, tolerance));
    CHECK_THAT(atium::act_360(start, end),
               Catch::Matchers::WithinAbs(0.08333333333333333, tolerance));
    CHECK_THAT(atium::thirty_360_bb(start, end),
               Catch::Matchers::WithinAbs(0.08333333333333333, tolerance));
    CHECK_THAT(atium::thirty_360_e(start, end),
               Catch::Matchers::WithinAbs(0.08333333333333333, tolerance));
}

TEST_CASE("31st to 31st") {
    const year_month_day start{2026y / January / 31};
    const year_month_day end{2026y / March / 31};
    CHECK(start.ok());
    CHECK(end.ok());
    CHECK((sys_days{end} - sys_days{start}).count() == 59);

    CHECK_THAT(atium::act_365f(start, end),
               Catch::Matchers::WithinAbs(0.16164383561643836, tolerance));
    CHECK_THAT(atium::act_360(start, end),
               Catch::Matchers::WithinAbs(0.1638888888888889, tolerance));
    CHECK_THAT(atium::thirty_360_bb(start, end),
               Catch::Matchers::WithinAbs(0.16666666666666666, tolerance));
    CHECK_THAT(atium::thirty_360_e(start, end),
               Catch::Matchers::WithinAbs(0.16666666666666666, tolerance));
}

TEST_CASE("into February") {
    const year_month_day start{2026y / January / 31};
    const year_month_day end{2026y / February / 28};
    CHECK(start.ok());
    CHECK(end.ok());
    CHECK((sys_days{end} - sys_days{start}).count() == 28);

    CHECK_THAT(atium::act_365f(start, end),
               Catch::Matchers::WithinAbs(0.07671232876712329, tolerance));
    CHECK_THAT(atium::act_360(start, end),
               Catch::Matchers::WithinAbs(0.07777777777777778, tolerance));
    CHECK_THAT(atium::thirty_360_bb(start, end),
               Catch::Matchers::WithinAbs(0.07777777777777778, tolerance));
    CHECK_THAT(atium::thirty_360_e(start, end),
               Catch::Matchers::WithinAbs(0.07777777777777778, tolerance));
}

TEST_CASE("crosses a year boundary") {
    const year_month_day start{2026y / November / 30};
    const year_month_day end{2027y / February / 28};
    CHECK(start.ok());
    CHECK(end.ok());
    CHECK((sys_days{end} - sys_days{start}).count() == 90);

    CHECK_THAT(atium::act_365f(start, end),
               Catch::Matchers::WithinAbs(0.2465753424657534, tolerance));
    CHECK_THAT(atium::act_360(start, end), Catch::Matchers::WithinAbs(0.25, tolerance));
    CHECK_THAT(atium::thirty_360_bb(start, end),
               Catch::Matchers::WithinAbs(0.24444444444444444, tolerance));
    CHECK_THAT(atium::thirty_360_e(start, end),
               Catch::Matchers::WithinAbs(0.24444444444444444, tolerance));
}

TEST_CASE("same day; must be 0, not an error") {
    const year_month_day start{2026y / June / 15};
    const year_month_day end{2026y / June / 15};
    CHECK(start.ok());
    CHECK(end.ok());
    CHECK((sys_days{end} - sys_days{start}).count() == 0);

    CHECK_THAT(atium::act_365f(start, end), Catch::Matchers::WithinAbs(0.0, tolerance));
    CHECK_THAT(atium::act_360(start, end), Catch::Matchers::WithinAbs(0.0, tolerance));
    CHECK_THAT(atium::thirty_360_bb(start, end), Catch::Matchers::WithinAbs(0.0, tolerance));
    CHECK_THAT(atium::thirty_360_e(start, end), Catch::Matchers::WithinAbs(0.0, tolerance));
}

TEST_CASE("distinct dates worth zero under 30/360") {
    const year_month_day start{2026y / January / 30};
    const year_month_day end{2026y / January / 31};
    CHECK(start.ok());
    CHECK(end.ok());
    CHECK((sys_days{end} - sys_days{start}).count() == 1);

    CHECK_THAT(atium::act_365f(start, end),
               Catch::Matchers::WithinAbs(0.0027397260273972603, tolerance));
    CHECK_THAT(atium::act_360(start, end),
               Catch::Matchers::WithinAbs(0.002777777777777778, tolerance));
    CHECK_THAT(atium::thirty_360_bb(start, end), Catch::Matchers::WithinAbs(0.0, tolerance));
    CHECK_THAT(atium::thirty_360_e(start, end), Catch::Matchers::WithinAbs(0.0, tolerance));
}

TEST_CASE("33 fictional days in a 30-day month; BB != 30E") {
    const year_month_day start{2026y / February / 28};
    const year_month_day end{2026y / March / 31};
    CHECK(start.ok());
    CHECK(end.ok());
    CHECK((sys_days{end} - sys_days{start}).count() == 31);

    CHECK_THAT(atium::act_365f(start, end),
               Catch::Matchers::WithinAbs(0.08493150684931507, tolerance));
    CHECK_THAT(atium::act_360(start, end),
               Catch::Matchers::WithinAbs(0.08611111111111111, tolerance));
    CHECK_THAT(atium::thirty_360_bb(start, end),
               Catch::Matchers::WithinAbs(0.09166666666666666, tolerance));
    CHECK_THAT(atium::thirty_360_e(start, end),
               Catch::Matchers::WithinAbs(0.08888888888888889, tolerance));
}

TEST_CASE("leap-day start, tau > half a year; BB != 30E") {
    const year_month_day start{2028y / February / 29};
    const year_month_day end{2028y / August / 31};
    CHECK(start.ok());
    CHECK(end.ok());
    CHECK((sys_days{end} - sys_days{start}).count() == 184);

    CHECK_THAT(atium::act_365f(start, end),
               Catch::Matchers::WithinAbs(0.5041095890410959, tolerance));
    CHECK_THAT(atium::act_360(start, end),
               Catch::Matchers::WithinAbs(0.5111111111111111, tolerance));
    CHECK_THAT(atium::thirty_360_bb(start, end),
               Catch::Matchers::WithinAbs(0.5055555555555555, tolerance));
    CHECK_THAT(atium::thirty_360_e(start, end),
               Catch::Matchers::WithinAbs(0.5027777777777778, tolerance));
}
