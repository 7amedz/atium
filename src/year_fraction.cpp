#include "atium/year_fraction.hpp"

#include <chrono>

namespace atium {

// year fraction functions for each convention

// will thorw runtime_error and end the run on impossible data/wrong math

// actual 365 fixed year fraction convention
// (d2-d1)/365
double act_365f(std::chrono::year_month_day d1, std::chrono::year_month_day d2) {

    // sys_days is a time point
    // can call sd.time_since_epoch() which returns std::chrono::days
    // sd.time_since_epoch().count() returns long
    //
    //  sd - sd (overloaded minus) returns std::chrono::days on which we can call .count() insta

    const auto day_count = (std::chrono::sys_days{d2} - std::chrono::sys_days{d1}).count();
    if (day_count < 0) {
        throw std::runtime_error(std::format(
            "Negative day count while computing year fraction in act_365f \nDates: d1:{} \n d2: {}",
            d1, d2));
    }

    return static_cast<double>(day_count) / 365.0;
}
} // namespace atium
