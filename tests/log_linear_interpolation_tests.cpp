#include <limits>
#include <stdexcept>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "atium/rates.hpp"

constexpr double relative_tolerance{1e-12};

TEST_CASE("log_linear_factor interpolates a declining pair") {
    CHECK_THAT(atium::log_linear_factor(0.99, 0.97, 0.5),
               Catch::Matchers::WithinRel(0.9799489782636644, relative_tolerance));
}

TEST_CASE("log_linear_factor accepts inverted factors") {
    CHECK_THAT(atium::log_linear_factor(0.98, 1.02, 0.25),
               Catch::Matchers::WithinRel(0.9898504838590922, relative_tolerance));
}

TEST_CASE("log_linear_factor returns exact endpoint factors") {
    constexpr double start_factor{1.005};
    constexpr double end_factor{0.995};

    CHECK(atium::log_linear_factor(start_factor, end_factor, 0.0) == start_factor);
    CHECK(atium::log_linear_factor(start_factor, end_factor, 1.0) == end_factor);
}

TEST_CASE("log_linear_factor rejects invalid inputs") {
    constexpr double valid_start_factor{0.99};
    constexpr double valid_end_factor{0.97};

    CHECK_THROWS_AS(atium::log_linear_factor(0.0, valid_end_factor, 0.5), std::runtime_error);
    CHECK_THROWS_AS(atium::log_linear_factor(-0.99, valid_end_factor, 0.5),
                    std::runtime_error);
    CHECK_THROWS_AS(
        atium::log_linear_factor(std::numeric_limits<double>::infinity(), valid_end_factor, 0.5),
        std::runtime_error);
    CHECK_THROWS_AS(atium::log_linear_factor(std::numeric_limits<double>::quiet_NaN(),
                                              valid_end_factor, 0.5),
                    std::runtime_error);
    CHECK_THROWS_AS(atium::log_linear_factor(valid_start_factor, 0.0, 0.5), std::runtime_error);
    CHECK_THROWS_AS(atium::log_linear_factor(valid_start_factor, -0.97, 0.5),
                    std::runtime_error);
    CHECK_THROWS_AS(
        atium::log_linear_factor(valid_start_factor, std::numeric_limits<double>::infinity(), 0.5),
        std::runtime_error);
    CHECK_THROWS_AS(atium::log_linear_factor(valid_start_factor,
                                              std::numeric_limits<double>::quiet_NaN(), 0.5),
                    std::runtime_error);
    CHECK_THROWS_AS(atium::log_linear_factor(valid_start_factor, valid_end_factor, -0.01),
                    std::runtime_error);
    CHECK_THROWS_AS(atium::log_linear_factor(valid_start_factor, valid_end_factor, 1.01),
                    std::runtime_error);
    CHECK_THROWS_AS(atium::log_linear_factor(valid_start_factor, valid_end_factor,
                                              std::numeric_limits<double>::infinity()),
                    std::runtime_error);
    CHECK_THROWS_AS(atium::log_linear_factor(valid_start_factor, valid_end_factor,
                                              std::numeric_limits<double>::quiet_NaN()),
                    std::runtime_error);
}
