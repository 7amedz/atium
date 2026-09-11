#include "atium/cashflow.hpp"

#include <cmath>
#include <cstddef>
#include <format>
#include <stdexcept>
#include <vector>

#include "atium/coupon.hpp"

namespace atium {

double present_value(const Cashflow& cashflow, double discount_factor) {

    if (std::isnan(discount_factor)) {
        throw std::runtime_error("Error getting present_value \ndiscount_factor is not a number\n");
    }

    if (discount_factor <= 0) {
        throw std::runtime_error(
            "Error getting present_value \ndiscount_factor is not strictly positive\n");
    }
    if (!std::isfinite(discount_factor)) {
        throw std::runtime_error("Error getting present_value \ndiscount_factor is not finite\n");
    }

    return cashflow.amount * discount_factor;
}

std::vector<Cashflow> fixed_cashflows(const std::vector<std::chrono::year_month_day>& schedule,
                                      double notional, double fixed_rate,
                                      const std::vector<double>& year_fractions) {

    if (schedule.size() < 2) {
        throw std::runtime_error(std::format("Error getting fixed_cashflows\nschedule needs at "
                                             "least two dates\nschedule.size() = {}\n",
                                             schedule.size()));
    }
    if (year_fractions.size() != schedule.size() - 1) {
        throw std::runtime_error(std::format(
            "Error getting fixed_cashflows\nyear_fractions size does not match schedule periods\n"
            "year_fractions.size() = {}\nschedule.size() - 1 = {}\n",
            year_fractions.size(), schedule.size() - 1));
    }
    for (std::size_t i{0}; i < schedule.size(); ++i) {
        if (!schedule[i].ok()) {
            throw std::runtime_error(std::format(
                "Error getting fixed_cashflows\nschedule contains an invalid date\n"
                "schedule[{}] = {}/{}/{}\n",
                i, static_cast<int>(schedule[i].year()), static_cast<unsigned>(schedule[i].month()),
                static_cast<unsigned>(schedule[i].day())));
        }
        if (i > 0) {
            if (schedule[i] <= schedule[i - 1]) {
                throw std::runtime_error(std::format(
                    "Error getting fixed_cashflows\nschedule dates are not strictly increasing\n"
                    "schedule[{}] = {}/{}/{}\nschedule[{}] = {}/{}/{}\n",
                    i - 1, static_cast<int>(schedule[i - 1].year()),
                    static_cast<unsigned>(schedule[i - 1].month()),
                    static_cast<unsigned>(schedule[i - 1].day()), i,
                    static_cast<int>(schedule[i].year()),
                    static_cast<unsigned>(schedule[i].month()),
                    static_cast<unsigned>(schedule[i].day())));
            }
        }
    }

    std::vector<Cashflow> cashflows;
    cashflows.reserve(year_fractions.size());

    for (std::size_t i{1}; i < schedule.size(); i++) {
        cashflows.push_back({.payment_date = schedule[i],
                             .amount = coupon_amount(notional, fixed_rate, year_fractions[i - 1])});
    }
    return cashflows;
}

std::vector<Cashflow> floating_cashflows(const std::vector<std::chrono::year_month_day>& schedule,
                                         double notional, const std::vector<double>& floating_rates,
                                         const std::vector<double>& year_fractions) {

    if (schedule.size() < 2) {
        throw std::runtime_error(
            std::format("Error getting floating_cashflows\nschedule needs at least two dates\n"
                        "schedule.size() = {}\n",
                        schedule.size()));
    }
    if (floating_rates.size() != schedule.size() - 1) {
        throw std::runtime_error(
            std::format("Error getting floating_cashflows\nfloating_rates size does not match "
                        "schedule periods\n"
                        "floating_rates.size() = {}\nschedule.size() - 1 = {}\n",
                        floating_rates.size(), schedule.size() - 1));
    }
    if (year_fractions.size() != schedule.size() - 1) {
        throw std::runtime_error(
            std::format("Error getting floating_cashflows\nyear_fractions size does not match "
                        "schedule periods\n"
                        "year_fractions.size() = {}\nschedule.size() - 1 = {}\n",
                        year_fractions.size(), schedule.size() - 1));
    }
    for (std::size_t i{0}; i < schedule.size(); ++i) {
        if (!schedule[i].ok()) {
            throw std::runtime_error(std::format(
                "Error getting floating_cashflows\nschedule contains an invalid date\n"
                "schedule[{}] = {}/{}/{}\n",
                i, static_cast<int>(schedule[i].year()), static_cast<unsigned>(schedule[i].month()),
                static_cast<unsigned>(schedule[i].day())));
        }
        if (i > 0 && schedule[i] <= schedule[i - 1]) {
            throw std::runtime_error(std::format(
                "Error getting floating_cashflows\nschedule dates are not strictly increasing\n"
                "schedule[{}] = {}/{}/{}\nschedule[{}] = {}/{}/{}\n",
                i - 1, static_cast<int>(schedule[i - 1].year()),
                static_cast<unsigned>(schedule[i - 1].month()),
                static_cast<unsigned>(schedule[i - 1].day()), i,
                static_cast<int>(schedule[i].year()), static_cast<unsigned>(schedule[i].month()),
                static_cast<unsigned>(schedule[i].day())));
        }
    }

    std::vector<Cashflow> cashflows{};
    cashflows.reserve(floating_rates.size());

    for (std::size_t i{1}; i < schedule.size(); ++i) {
        cashflows.push_back(
            {.payment_date = schedule[i],
             .amount = coupon_amount(notional, floating_rates[i - 1], year_fractions[i - 1])});
    }
    return cashflows;
}

} // namespace atium
