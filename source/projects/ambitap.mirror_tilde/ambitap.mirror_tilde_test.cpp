/// @file
/// @brief      Unit tests for ambitap.mirror~ (Min-level: attribute defaults).
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include "c74_min_unittest.h" // required unit-test header (defines main via Catch)
// The object source is included second on purpose: min-api requires the
// Catch header to come first, and "ambitap." sorts ahead of it.
#include "ambitap.mirror_tilde.cpp" // include the object source so we can instantiate it

SCENARIO("ambitap.mirror~ instantiates with the documented defaults") {
    ext_main(nullptr); // configure the class (required once per test executable)

    GIVEN("a default instance (no creation args: order falls back to 1)") {
        test_wrapper<ambitap_mirror> an_instance;
        ambitap_mirror&              my_object = an_instance;

        THEN("no plane is mirrored, so the object starts as a passthrough") {
            REQUIRE(static_cast<bool>(my_object.flip_lr) == false);
            REQUIRE(static_cast<bool>(my_object.flip_fb) == false);
            REQUIRE(static_cast<bool>(my_object.flip_ud) == false);
        }
    }
}

SCENARIO("ambitap.mirror~ tracks each mirror plane independently") {
    ext_main(nullptr);

    GIVEN("a default instance") {
        test_wrapper<ambitap_mirror> an_instance;
        ambitap_mirror&              my_object = an_instance;

        WHEN("only the left/right plane is enabled") {
            my_object.flip_lr = true;
            THEN("the other two planes are untouched") {
                REQUIRE(static_cast<bool>(my_object.flip_lr) == true);
                REQUIRE(static_cast<bool>(my_object.flip_fb) == false);
                REQUIRE(static_cast<bool>(my_object.flip_ud) == false);
            }
        }
    }
}
