/// @file
/// @brief      Unit tests for ambitap.rotate~ (Min-level: attribute defaults).
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include "c74_min_unittest.h" // required unit-test header (defines main via Catch)
// The object source is included second on purpose: min-api requires the
// Catch header to come first, and "ambitap." sorts ahead of it.
#include "ambitap.rotate_tilde.cpp" // include the object source so we can instantiate it

SCENARIO("ambitap.rotate~ instantiates with the documented defaults") {
    ext_main(nullptr); // configure the class (required once per test executable)

    GIVEN("a default instance (no creation args: order falls back to 1)") {
        test_wrapper<ambitap_rotate> an_instance;
        ambitap_rotate&              my_object = an_instance;

        THEN("all three Euler angles are zero, so the rotation starts as identity") {
            REQUIRE(static_cast<double>(my_object.yaw) == 0.0);
            REQUIRE(static_cast<double>(my_object.pitch) == 0.0);
            REQUIRE(static_cast<double>(my_object.roll) == 0.0);
        }
    }
}
