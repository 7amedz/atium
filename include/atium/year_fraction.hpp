#pragma once

#include <chrono>

namespace atium {

// actual 365 fixed year fraction convention
double act_365f(std::chrono::year_month_day d1, std::chrono::year_month_day d2);

// actual 360 yf convention
double act_360(std::chrono::year_month_day d1, std::chrono::year_month_day d2);

//  30/360bb (bond basis)
double thirty_360_bb(std::chrono::year_month_day d1, std::chrono::year_month_day d2);

//  30E/360 (Eurobond)
double thirty_360_e(std::chrono::year_month_day d1, std::chrono::year_month_day d2);

} // namespace atium
