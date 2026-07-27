/// @file
/// @brief      Unit tests for ambitap.directional~ (Min-level: attribute defaults).
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include "c74_min_unittest.h" // required unit-test header (defines main via Catch)
// The object source is included second on purpose: min-api requires the
// Catch header to come first, and "ambitap." sorts ahead of it.
#include "ambitap.directional_tilde.cpp" // include the object source so we can instantiate it

SCENARIO("ambitap.directional~ instantiates with the documented defaults") {
    ext_main(nullptr); // configure the class (required once per test executable)

    GIVEN("a default instance (no creation args: order falls back to 1)") {
        test_wrapper<ambitap_directional> an_instance;
        ambitap_directional&              my_object = an_instance;

        THEN("the look direction is dead ahead on the horizon") {
            REQUIRE(static_cast<double>(my_object.azimuth) == 0.0);
            REQUIRE(static_cast<double>(my_object.elevation) == 0.0);
        }
        THEN("the directional gain defaults to the documented bypass value of 1") {
            REQUIRE(static_cast<double>(my_object.gain) == 1.0);
        }
    }
}
