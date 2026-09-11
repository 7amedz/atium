#include "atium/cashflow.hpp"

#include <cmath>
#include <stdexcept>

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

} // namespace atium
