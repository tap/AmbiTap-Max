/// @file
/// @brief      Unit tests for ambitap.doppler~ (Min-level: attribute defaults).
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include "c74_min_unittest.h" // required unit-test header (defines main via Catch)
// The object source is included second on purpose: min-api requires the
// Catch header to come first, and "ambitap." sorts ahead of it.
#include "ambitap.doppler_tilde.cpp" // include the object source so we can instantiate it

SCENARIO("ambitap.doppler~ instantiates with the documented defaults") {
    ext_main(nullptr); // configure the class (required once per test executable)

    GIVEN("a default instance (no creation args: order falls back to 1)") {
        test_wrapper<ambitap_doppler> an_instance;
        ambitap_doppler&              my_object = an_instance;

        THEN("the source sits one meter out") {
            REQUIRE(static_cast<double>(my_object.distance) == 1.0);
        }
        THEN("the time-of-flight model uses 343 m/s") {
            REQUIRE(static_cast<double>(my_object.speed_of_sound) == 343.0);
        }
        THEN("the delay buffer is sized for 50 m") {
            REQUIRE(static_cast<double>(my_object.max_distance) == 50.0);
        }
    }
}
