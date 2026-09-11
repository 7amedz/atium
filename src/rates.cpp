
#include "atium/rates.hpp"

#include <cmath>
#include <format>
#include <stdexcept>

namespace atium {

double simple_discount_factor(double simple_rate, double year_fraction) {
    if (!std::isfinite(simple_rate)) {
        throw std::runtime_error(std::format(
            "Error getting simple_discount_factor\nsimple_rate is not finite\nsimple_rate = {}\n",
            simple_rate));
    }
    if (!std::isfinite(year_fraction)) {
        throw std::runtime_error(std::format("Error getting simple_discount_factor\nyear_fraction "
                                             "is not finite\nyear_fraction = {}\n",
                                             year_fraction));
    }
    if (year_fraction <= 0.0) {
        throw std::runtime_error(std::format(
            "Error getting simple_discount_factor\nyear_fraction is not strictly positive\n"
            "year_fraction = {}\n",
            year_fraction));
    }
    const double denominator = 1.0 + (simple_rate * year_fraction);
    if (!std::isfinite(denominator)) {
        throw std::runtime_error(std::format(
            "Error getting simple_discount_factor\ndenominator is not finite\ndenominator = {}\n"
            "simple_rate = {}\nyear_fraction = {}\n",
            denominator, simple_rate, year_fraction));
    }
    if (denominator <= 0.0) {
        throw std::runtime_error(std::format(
            "Error getting simple_discount_factor\ndenominator is not strictly positive\n"
            "denominator = {}\nsimple_rate = {}\nyear_fraction = {}\n",
            denominator, simple_rate, year_fraction));
    }
    return 1.0 / denominator;
}

double simple_forward_rate(double start_factor, double end_factor, double year_fraction) {
    if (!std::isfinite(start_factor)) {
        throw std::runtime_error(std::format(
            "Error getting simple_forward_rate\nstart_factor is not finite\nstart_factor = {}\n",
            start_factor));
    }
    if (start_factor <= 0.0) {
        throw std::runtime_error(
            std::format("Error getting simple_forward_rate\nstart_factor is not strictly positive\n"
                        "start_factor = {}\n",
                        start_factor));
    }
    if (!std::isfinite(end_factor)) {
        throw std::runtime_error(std::format(
            "Error getting simple_forward_rate\nend_factor is not finite\nend_factor = {}\n",
            end_factor));
    }
    if (end_factor <= 0.0) {
        throw std::runtime_error(
            std::format("Error getting simple_forward_rate\nend_factor is not strictly positive\n"
                        "end_factor = {}\n",
                        end_factor));
    }
    if (!std::isfinite(year_fraction)) {
        throw std::runtime_error(std::format(
            "Error getting simple_forward_rate\nyear_fraction is not finite\nyear_fraction = {}\n",
            year_fraction));
    }
    if (year_fraction <= 0.0) {
        throw std::runtime_error(std::format(
            "Error getting simple_forward_rate\nyear_fraction is not strictly positive\n"
            "year_fraction = {}\n",
            year_fraction));
    }

    return ((start_factor / end_factor) - 1.0) / year_fraction;
}

} // namespace atium
