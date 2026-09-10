#include "atium/schedule.hpp"

#include <algorithm>
#include <chrono>
#include <stdexcept>
#include <vector>

namespace atium {

// take termination_date as base and make back jumps = frequency for each iteration
//              from termination_date
// frequency in months
// we keep the termination_date.day as preferred day to use for ymds in the schedule.
// In case (termination_date - frequency) < start_date (checked on i=1) we just end
//              up with schedule being the two input dates
// If we overshoot start date (same year_month) add start date and break the loop
//                      else (different year_month) we throw
std::vector<Ymd> generate_schedule(Ymd start_date, Ymd termination_date, DurationM frequency) {

    if (!start_date.ok() || !termination_date.ok()) {
        throw std::runtime_error(std::format(
            "Invalid dates in schedule generation \nDates: start_date:{} \n termination_date: {}",
            start_date, termination_date));
    }

    if (!(start_date < termination_date)) {
        throw std::runtime_error(
            std::format("Termination date is not after start date in schedule generation \nDates: "
                        "start_date:{} \n termination_date: {}",
                        start_date, termination_date));
    }

    if (frequency.count() <= 0) {
        throw std::runtime_error(
            std::format("Frequency is not positive in schedule generation \nFrequency: {}M",
                        frequency.count()));
    }

    const std::chrono::year_month termination_ym =
        std::chrono::year_month{termination_date.year(), termination_date.month()};
    const std::chrono::year_month start_ym =
        std::chrono::year_month{start_date.year(), start_date.month()};
    std::vector<Ymd> schedule;

    for (int i{};; i++) {
        auto ym = termination_ym - (i * frequency);
        auto ymd = std::chrono::year_month_day{
            ym.year(), ym.month(),
            std::min(termination_date.day(), (ym / std::chrono::last).day())};
        if (ymd < start_date) {

            // year_month overshoot
            // i>1: if first iteration overshoots it means the period between start and
            //      termination is less than the frequency in which case we take the two
            //      input dates as the schedule(agrees with quantlib)
            if (ym != start_ym && i > 1) {

                // format("{}",ymd) gives the date formatted nicely
                throw std::runtime_error(std::format(
                    "Error in schedule generation. Overshoot happened while stepping back from "
                    "termination_date \nstart_date:{} \n termination_date: {}\n "
                    "frequency: {}M\n",
                    start_date, termination_date, frequency.count()));
            }

            schedule.push_back(start_date); // day overshoot. we take start_date.day
            break;
        }

        // add the new date
        schedule.push_back(ymd);

        if (ymd == start_date) { // duh
            break;
        }
    }

    // since we walked back, the schedule is written backwards
    //  std::reverse(schedule.begin(), schedule.end());
    std::ranges::reverse(schedule);
    return schedule;
}

} // namespace atium
