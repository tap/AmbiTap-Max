/// @file
/// @brief      Unit tests for ambitap.compress~ (Min-level: attribute defaults).
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include "c74_min_unittest.h" // required unit-test header (defines main via Catch)
// The object source is included second on purpose: min-api requires the
// Catch header to come first, and "ambitap." sorts ahead of it.
#include "ambitap.compress_tilde.cpp" // include the object source so we can instantiate it

SCENARIO("ambitap.compress~ instantiates with the documented defaults") {
    ext_main(nullptr); // configure the class (required once per test executable)

    GIVEN("a default instance (no creation args: order falls back to 1)") {
        test_wrapper<ambitap_compress> an_instance;
        ambitap_compress&              my_object = an_instance;

        THEN("the detector threshold and ratio are the documented 4:1 above -12 dB") {
            REQUIRE(static_cast<double>(my_object.threshold) == -12.0);
            REQUIRE(static_cast<double>(my_object.ratio) == 4.0);
        }
        THEN("the envelope times are 5 ms attack / 100 ms release") {
            REQUIRE(static_cast<double>(my_object.attack) == 0.005);
            REQUIRE(static_cast<double>(my_object.release) == 0.1);
        }
        THEN("no makeup gain is applied") {
            REQUIRE(static_cast<double>(my_object.makeup_gain) == 0.0);
        }
    }
}
