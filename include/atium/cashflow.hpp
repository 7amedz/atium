#pragma once

#include <chrono>
namespace atium {

struct Cashflow {
    std::chrono::year_month_day payment_date;
    double amount;
};

double present_value(const Cashflow& cashflow, double discount_factor);

} // namespace atium
