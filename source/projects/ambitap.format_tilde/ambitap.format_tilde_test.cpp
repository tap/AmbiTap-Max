/// @file
/// @brief      Unit tests for ambitap.format~ (Min-level: attribute defaults).
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include "c74_min_unittest.h" // required unit-test header (defines main via Catch)
// The object source is included second on purpose: min-api requires the
// Catch header to come first, and "ambitap." sorts ahead of it.
#include "ambitap.format_tilde.cpp" // include the object source so we can instantiate it

SCENARIO("ambitap.format~ instantiates with the documented defaults") {
    ext_main(nullptr); // configure the class (required once per test executable)

    GIVEN("a default instance (no creation args: order falls back to 1)") {
        test_wrapper<ambitap_format> an_instance;
        ambitap_format&              my_object = an_instance;

        THEN("the conversion runs AmbiX in, FuMa out") {
            REQUIRE(my_object.direction == symbol("ambix_to_fuma"));
        }
    }
}

SCENARIO("ambitap.format~ accepts the reverse conversion direction") {
    ext_main(nullptr);

    GIVEN("a default instance") {
        test_wrapper<ambitap_format> an_instance;
        ambitap_format&              my_object = an_instance;

        WHEN("the FuMa-to-AmbiX direction is requested") {
            my_object.direction = symbol("fuma_to_ambix");
            THEN("the attribute reports it") {
                REQUIRE(my_object.direction == symbol("fuma_to_ambix"));
            }
        }
    }
}
