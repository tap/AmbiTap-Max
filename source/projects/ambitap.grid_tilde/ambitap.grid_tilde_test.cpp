/// @file
/// @brief      Unit tests for ambitap.grid~ (Min-level: attribute defaults, clamping, rounding).
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include "c74_min_unittest.h" // required unit-test header (defines main via Catch)
// The object source is included second on purpose: min-api requires the
// Catch header to come first, and "ambitap." sorts ahead of it.
#include "ambitap.grid_tilde.cpp" // include the object source so we can instantiate it

SCENARIO("ambitap.grid~ instantiates with the documented defaults") {
    ext_main(nullptr); // configure the class (required once per test executable)

    GIVEN("a default instance (no creation args: order falls back to 1)") {
        test_wrapper<ambitap_grid> an_instance;
        ambitap_grid&              my_object = an_instance;

        THEN("the heatmap is 32 columns wide (rows are half that)") {
            REQUIRE(static_cast<int>(my_object.azimuth_steps) == 32);
        }
        THEN("per-direction energy is smoothed over 200 ms") {
            REQUIRE(static_cast<double>(my_object.smoothing_time) == 200.0);
        }
        THEN("the display window is 40 dB below the peak") {
            REQUIRE(static_cast<double>(my_object.dynamic_range) == 40.0);
        }
    }
}

SCENARIO("ambitap.grid~ clamps the azimuth resolution to 4..128 and rounds it to even") {
    ext_main(nullptr);

    GIVEN("a default instance") {
        test_wrapper<ambitap_grid> an_instance;
        ambitap_grid&              my_object = an_instance;

        WHEN("an odd resolution inside the range is requested") {
            my_object.azimuth_steps = 7;
            THEN("it rounds down to the next even value") {
                REQUIRE(static_cast<int>(my_object.azimuth_steps) == 6);
            }
        }
        WHEN("a resolution above the 128-column ceiling is requested") {
            my_object.azimuth_steps = 1000;
            THEN("it clamps down to the ceiling") {
                REQUIRE(static_cast<int>(my_object.azimuth_steps) == 128);
            }
        }
        WHEN("a resolution below the 4-column floor is requested") {
            my_object.azimuth_steps = 1;
            THEN("it clamps up to the floor") {
                REQUIRE(static_cast<int>(my_object.azimuth_steps) == 4);
            }
        }
        WHEN("an even resolution inside the range is requested") {
            my_object.azimuth_steps = 64;
            THEN("it is taken as-is") {
                REQUIRE(static_cast<int>(my_object.azimuth_steps) == 64);
            }
        }
    }
}
