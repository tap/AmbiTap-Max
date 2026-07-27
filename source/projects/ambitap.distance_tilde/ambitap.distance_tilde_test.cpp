/// @file
/// @brief      Unit tests for ambitap.distance~ (Min-level: attribute defaults, clamping).
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include "c74_min_unittest.h" // required unit-test header (defines main via Catch)
// The object source is included second on purpose: min-api requires the
// Catch header to come first, and "ambitap." sorts ahead of it.
#include "ambitap.distance_tilde.cpp" // include the object source so we can instantiate it

SCENARIO("ambitap.distance~ instantiates with the documented defaults") {
    ext_main(nullptr); // configure the class (required once per test executable)

    GIVEN("a default instance (no creation args: order falls back to 1)") {
        test_wrapper<ambitap_distance> an_instance;
        ambitap_distance&              my_object = an_instance;

        THEN("the source sits at the reference distance, so every cue is identity") {
            REQUIRE(static_cast<double>(my_object.distance) == 1.0);
            REQUIRE(static_cast<double>(my_object.reference_distance) == 1.0);
        }
        THEN("the distance-gain exponent is the physical 1/r law") {
            REQUIRE(static_cast<double>(my_object.attenuation) == 1.0);
        }
        THEN("air absorption is off (opt-in)") {
            REQUIRE(static_cast<double>(my_object.air_absorption) == 0.0);
        }
        THEN("the propagation model uses 343 m/s over a 50 m buffer") {
            REQUIRE(static_cast<double>(my_object.speed_of_sound) == 343.0);
            REQUIRE(static_cast<double>(my_object.max_distance) == 50.0);
        }
        THEN("both the delay and the near-field compensation are enabled") {
            REQUIRE(static_cast<bool>(my_object.doppler_enabled) == true);
            REQUIRE(static_cast<bool>(my_object.nfc_enabled) == true);
        }
    }
}

SCENARIO("ambitap.distance~ clamps the attenuation exponent to non-negative") {
    ext_main(nullptr);

    GIVEN("a default instance") {
        test_wrapper<ambitap_distance> an_instance;
        ambitap_distance&              my_object = an_instance;

        WHEN("a negative exponent is requested (gain rising with distance)") {
            my_object.attenuation = -2.0;
            THEN("it clamps to zero, which disables the distance gain") {
                REQUIRE(static_cast<double>(my_object.attenuation) == 0.0);
            }
        }
        WHEN("a steeper-than-physical exponent is requested") {
            my_object.attenuation = 2.0;
            THEN("it is taken as-is (there is no upper bound)") {
                REQUIRE(static_cast<double>(my_object.attenuation) == 2.0);
            }
        }
    }
}

SCENARIO("ambitap.distance~ clamps the air-absorption amount to 0..1") {
    ext_main(nullptr);

    GIVEN("a default instance") {
        test_wrapper<ambitap_distance> an_instance;
        ambitap_distance&              my_object = an_instance;

        WHEN("an amount above the documented range is requested") {
            my_object.air_absorption = 5.0;
            THEN("it clamps down to 1") {
                REQUIRE(static_cast<double>(my_object.air_absorption) == 1.0);
            }
        }
        WHEN("a negative amount is requested") {
            my_object.air_absorption = -1.0;
            THEN("it clamps up to 0") {
                REQUIRE(static_cast<double>(my_object.air_absorption) == 0.0);
            }
        }
        WHEN("an amount inside the range is requested") {
            my_object.air_absorption = 0.25;
            THEN("it is taken as-is") {
                REQUIRE(static_cast<double>(my_object.air_absorption) == 0.25);
            }
        }
    }
}
