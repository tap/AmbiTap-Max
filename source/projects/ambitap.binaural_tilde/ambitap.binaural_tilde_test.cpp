/// @file
/// @brief      Unit tests for ambitap.binaural~ (Min-level: attribute defaults).
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include "c74_min_unittest.h" // required unit-test header (defines main via Catch)
// The object source is included second on purpose: min-api requires the
// Catch header to come first, and "ambitap." sorts ahead of it.
#include "ambitap.binaural_tilde.cpp" // include the object source so we can instantiate it

SCENARIO("ambitap.binaural~ instantiates with the documented defaults") {
    ext_main(nullptr); // configure the class (required once per test executable)

    GIVEN("a default instance (no creation args: order falls back to 1)") {
        test_wrapper<ambitap_binaural> an_instance;
        ambitap_binaural&              my_object = an_instance;

        THEN("the post-convolution gain is unity") {
            REQUIRE(static_cast<double>(my_object.volume) == 1.0);
        }
        THEN("the built-in HRTF uses the least-squares projection") {
            REQUIRE(my_object.hrtf_dataset == symbol("ls"));
        }
        THEN("no SOFA file is loaded, so the built-in KEMAR set is in play") {
            REQUIRE(my_object.sofa == symbol(""));
        }
        THEN("head tracking starts at the identity orientation") {
            REQUIRE(static_cast<double>(my_object.yaw) == 0.0);
            REQUIRE(static_cast<double>(my_object.pitch) == 0.0);
            REQUIRE(static_cast<double>(my_object.roll) == 0.0);
        }
    }
}

SCENARIO("ambitap.binaural~ accepts the magnitude-least-squares HRTF projection") {
    ext_main(nullptr);

    GIVEN("a default instance") {
        test_wrapper<ambitap_binaural> an_instance;
        ambitap_binaural&              my_object = an_instance;

        WHEN("the magLS projection is requested") {
            my_object.hrtf_dataset = symbol("magls");
            THEN("the attribute reports it") {
                REQUIRE(my_object.hrtf_dataset == symbol("magls"));
            }
        }
    }
}
