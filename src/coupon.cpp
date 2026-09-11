#include "atium/coupon.hpp"

#include <cmath>
#include <format>
#include <stdexcept>

namespace atium {

double coupon_amount(double notional, double rate, double year_fraction) {
    if (!std::isfinite(notional)) {
        throw std::runtime_error(std::format(
            "Error getting coupon_amount\nnotional is not finite\nnotional = {}\n", notional));
    }
    if (notional <= 0.0) {
        throw std::runtime_error(std::format(
            "Error getting coupon_amount\nnotional is not strictly positive\nnotional = {}\n",
            notional));
    }
    if (!std::isfinite(rate)) {
        throw std::runtime_error(
            std::format("Error getting coupon_amount\nrate is not finite\nrate = {}\n", rate));
    }
    if (!std::isfinite(year_fraction)) {
        throw std::runtime_error(std::format(
            "Error getting coupon_amount\nyear_fraction is not finite\nyear_fraction = {}\n",
            year_fraction));
    }
    if (year_fraction <= 0.0) {
        throw std::runtime_error(
            std::format("Error getting coupon_amount\nyear_fraction is not strictly positive\n"
                        "year_fraction = {}\n",
                        year_fraction));
    }

    const double coupon = notional * (rate * year_fraction);
    if (!std::isfinite(coupon)) {
        throw std::runtime_error(std::format(
            "Error getting coupon_amount\ncoupon is not finite\ncoupon = {}\nnotional = {}\n"
            "rate = {}\nyear_fraction = {}\n",
            coupon, notional, rate, year_fraction));
    }

    return coupon;
}

} // namespace atium
