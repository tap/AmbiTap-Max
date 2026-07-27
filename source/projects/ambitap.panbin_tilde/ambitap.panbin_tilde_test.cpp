/// @file
/// @brief      Unit tests for ambitap.panbin~ (Min-level: attribute defaults).
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include "c74_min_unittest.h" // required unit-test header (defines main via Catch)
// The object source is included second on purpose: min-api requires the
// Catch header to come first, and "ambitap." sorts ahead of it.
#include "ambitap.panbin_tilde.cpp" // include the object source so we can instantiate it

SCENARIO("ambitap.panbin~ instantiates with the documented defaults") {
    ext_main(nullptr); // configure the class (required once per test executable)

    GIVEN("a default instance (no dsp yet, so no convolver pair is built)") {
        test_wrapper<ambitap_panbin> an_instance;
        ambitap_panbin&              my_object = an_instance;

        THEN("the source sits dead ahead on the horizon") {
            REQUIRE(static_cast<double>(my_object.azimuth) == 0.0);
            REQUIRE(static_cast<double>(my_object.elevation) == 0.0);
        }
        THEN("the post-convolution gain is unity") {
            REQUIRE(static_cast<double>(my_object.gain) == 1.0);
        }
    }
}
