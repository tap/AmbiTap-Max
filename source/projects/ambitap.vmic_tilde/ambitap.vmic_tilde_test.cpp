/// @file
/// @brief      Unit tests for ambitap.vmic~ (Min-level: attribute defaults).
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include "c74_min_unittest.h" // required unit-test header (defines main via Catch)
// The object source is included second on purpose: min-api requires the
// Catch header to come first, and "ambitap." sorts ahead of it.
#include "ambitap.vmic_tilde.cpp" // include the object source so we can instantiate it

SCENARIO("ambitap.vmic~ instantiates with the documented defaults") {
    ext_main(nullptr); // configure the class (required once per test executable)

    GIVEN("a default instance (no creation args: order falls back to 1)") {
        test_wrapper<ambitap_vmic> an_instance;
        ambitap_vmic&              my_object = an_instance;

        THEN("the virtual mic looks dead ahead on the horizon") {
            REQUIRE(static_cast<double>(my_object.azimuth) == 0.0);
            REQUIRE(static_cast<double>(my_object.elevation) == 0.0);
        }
        THEN("max-rE weighting is off, so the order-1 beam is the plain cardioid") {
            REQUIRE(static_cast<bool>(my_object.max_re) == false);
        }
    }
}
