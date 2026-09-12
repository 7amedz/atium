#pragma once

namespace atium {

double simple_discount_factor(double simple_rate, double year_fraction);
double simple_forward_rate(double start_factor, double end_factor, double year_fraction);
double log_linear_factor(double start_factor, double end_factor, double interpolation_fraction);

} // namespace atium
