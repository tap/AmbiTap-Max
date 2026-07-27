/// @file
/// @brief      Unit tests for ambitap.energyvec~ (Min-level: attribute defaults).
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include "c74_min_unittest.h" // required unit-test header (defines main via Catch)
// The object source is included second on purpose: min-api requires the
// Catch header to come first, and "ambitap." sorts ahead of it.
#include "ambitap.energyvec_tilde.cpp" // include the object source so we can instantiate it

SCENARIO("ambitap.energyvec~ instantiates with the documented defaults") {
    ext_main(nullptr); // configure the class (required once per test executable)

    GIVEN("a default instance") {
        test_wrapper<ambitap_energyvec> an_instance;
        ambitap_energyvec&              my_object = an_instance;

        THEN("the intensity smoothing time constant is the documented 10 ms") {
            REQUIRE(static_cast<double>(my_object.smoothing_time) == 0.01);
        }
    }
}
