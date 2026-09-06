#include "atium/year_fraction.hpp"

#include <chrono>
#include <cstdint>

namespace atium {

// year fraction functions for each convention

// will thorw runtime_error and end the run on impossible data/wrong math

// actual 365 fixed year fraction convention
// (d2-d1)/365
double act_365f(std::chrono::year_month_day d1, std::chrono::year_month_day d2) {

    // check if dates are valid
    if (!d1.ok() || !d2.ok()) {
        throw std::runtime_error(
            std::format("Invalid dates in act_365f \nDates: d1:{} \n d2: {}", d1, d2));
    }

    // check start<end
    if (d2 < d1) {
        throw std::runtime_error(std::format(
            "Second date is earlier than first in act_365f \nDates: d1:{} \n d2: {}", d1, d2));
    }

    // sys_days is a time point
    // can call sd.time_since_epoch() which returns std::chrono::days
    // sd.time_since_epoch().count() returns long
    //
    //  sd - sd (overloaded minus) returns std::chrono::days on which we can call .count() insta

    const auto day_count = (std::chrono::sys_days{d2} - std::chrono::sys_days{d1}).count();

    return static_cast<double>(day_count) / 365.0;
}

// ACT/360 year fraction convention
// (d2-d1)/360
double act_360(std::chrono::year_month_day d1, std::chrono::year_month_day d2) {

    // check if dates are valid
    if (!d1.ok() || !d2.ok()) {
        throw std::runtime_error(
            std::format("Invalid dates in act_360 \nDates: d1:{} \n d2: {}", d1, d2));
    }

    // check start<end
    if (d2 < d1) {
        throw std::runtime_error(std::format(
            "Second date is earlier than first in act_360 \nDates: d1:{} \n d2: {}", d1, d2));
    }

    const auto day_count = (std::chrono::sys_days{d2} - std::chrono::sys_days{d1}).count();

    return static_cast<double>(day_count) / 360.0;
}

//  30/360BB (bond basis)
//  if D1.day is 31 change it to 30
//  if D2.day is 31 and D1.day is 30 change D2.day to 30
//  day_count is = 360(y2-y1)+ 30(m2-m1)+(d2-d1)
double thirty_360_bb(std::chrono::year_month_day d1, std::chrono::year_month_day d2) {

    // check if dates are valid
    if (!d1.ok() || !d2.ok()) {
        throw std::runtime_error(
            std::format("Invalid dates in thirty_360_bb \nDates: d1:{} \n d2: {}", d1, d2));
    }

    // check start<end
    if (d2 < d1) {
        throw std::runtime_error(std::format(
            "Second date is earlier than first in thirty_360_bb \nDates: d1:{} \n d2: {}", d1, d2));
    }

    // day of the month corresponding to the dates
    // chrono::day also only supports uint cast
    int day_2 = static_cast<int>(static_cast<uint32_t>(d2.day()));
    int day_1 = static_cast<int>(static_cast<uint32_t>(d1.day()));

    if (day_1 == 31) {
        day_1 = 30;
    }

    if (day_2 == 31 && day_1 == 30) {
        day_2 = 30;
    }

    const int year_diff = static_cast<int>((d2.year() - d1.year()).count());

    // chrono::month can't be cast directly to int. needs to be unsigned first
    const int month_2 = static_cast<int>(static_cast<uint32_t>(d2.month()));
    const int month_1 = static_cast<int>(static_cast<uint32_t>(d1.month()));

    // month diff here can be negative (february - december) = -10
    const int month_diff = month_2 - month_1;

    // can be negative
    const int day_diff = day_2 - day_1;

    const int day_count = (360 * year_diff) + (30 * month_diff) + day_diff;

    return day_count / 360.0;
}

//  30E/360 (Eurobond)
//  if D1.day is 31 change it to 30
//  if D2.day is 31 change it to 30
//  day_count is = 360(y2-y1)+ 30(m2-m1)+(d2-d1)
double thirty_360_e(std::chrono::year_month_day d1, std::chrono::year_month_day d2) {

    // check if dates are valid
    if (!d1.ok() || !d2.ok()) {
        throw std::runtime_error(
            std::format("Invalid dates in thirty_360_e \nDates: d1:{} \n d2: {}", d1, d2));
    }

    // check start<end
    if (d2 < d1) {
        throw std::runtime_error(std::format(
            "Second date is earlier than first in thirty_360_e \nDates: d1:{} \n d2: {}", d1, d2));
    }

    // day of the month corresponding to the dates
    // chrono::day also only supports uint cast
    int day_2 = static_cast<int>(static_cast<uint32_t>(d2.day()));
    int day_1 = static_cast<int>(static_cast<uint32_t>(d1.day()));

    if (day_1 == 31) {
        day_1 = 30;
    }

    if (day_2 == 31) {
        day_2 = 30;
    }

    const int year_diff = static_cast<int>((d2.year() - d1.year()).count());

    // chrono::month can't be cast directly to int. needs to be unsigned first
    const int month_2 = static_cast<int>(static_cast<uint32_t>(d2.month()));
    const int month_1 = static_cast<int>(static_cast<uint32_t>(d1.month()));

    // month diff here can be negative (february - december) = -10
    const int month_diff = month_2 - month_1;

    // can be negative
    const int day_diff = day_2 - day_1;

    const int day_count = (360 * year_diff) + (30 * month_diff) + day_diff;

    return day_count / 360.0;
}

} // namespace atium
