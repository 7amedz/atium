#pragma once

#include <chrono>
#include <vector>

#include <bits/chrono.h>
namespace atium {

using Ymd = std::chrono::year_month_day;
using DurationM = std::chrono::months;

// Generate a list of dates that will be the paymen calender for a given trade.
// frequency: 3M or 6M or 12M for annual
// weekends/holidays not accounted for here.
std::vector<Ymd> generate_schedule(Ymd start_date, Ymd termination_date, DurationM frequency);

} // namespace atium
