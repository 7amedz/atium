#pragma once

#include <chrono>
#include <vector>
namespace atium {

struct Cashflow {
    std::chrono::year_month_day payment_date;
    double amount;
};

enum class PayReceive {
    Pay,
    Receive,
};

double present_value(const Cashflow& cashflow, double discount_factor);

double leg_present_value(const std::vector<Cashflow>& cashflows,
                         const std::vector<double>& discount_factors, PayReceive pay_receive);

std::vector<Cashflow> fixed_cashflows(const std::vector<std::chrono::year_month_day>& schedule,
                                      double notional, double fixed_rate,
                                      const std::vector<double>& year_fractions);

std::vector<Cashflow> floating_cashflows(const std::vector<std::chrono::year_month_day>& schedule,
                                         double notional, const std::vector<double>& floating_rates,
                                         const std::vector<double>& year_fractions);

} // namespace atium
