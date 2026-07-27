/// @file
/// @brief      Unit tests for ambitap.bed2hoa~ (Min-level: attribute defaults).
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include "c74_min_unittest.h" // required unit-test header (defines main via Catch)
// The object source is included second on purpose: min-api requires the
// Catch header to come first, and "ambitap." sorts ahead of it.
#include "ambitap.bed2hoa_tilde.cpp" // include the object source so we can instantiate it

SCENARIO("ambitap.bed2hoa~ instantiates with the documented defaults") {
    ext_main(nullptr); // configure the class (required once per test executable)

    GIVEN("a default instance (no creation args: order 1, surround_5_1 layout)") {
        test_wrapper<ambitap_bed2hoa> an_instance;
        ambitap_bed2hoa&              my_object = an_instance;

        THEN("the encoded bus is at unity gain") {
            REQUIRE(static_cast<double>(my_object.gain) == 1.0);
        }
    }
}
