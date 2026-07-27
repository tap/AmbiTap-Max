/// @file
/// @brief      Unit tests for ambitap.plate~ (Min-level: attribute defaults, clamping).
// SPDX-License-Identifier: MIT
// Copyright 2026 Timothy Place.

#include "c74_min_unittest.h" // required unit-test header (defines main via Catch)
// The object source is included second on purpose: min-api requires the
// Catch header to come first, and "ambitap." sorts ahead of it.
#include "ambitap.plate_tilde.cpp" // include the object source so we can instantiate it

SCENARIO("ambitap.plate~ instantiates with Dattorro's documented defaults") {
    ext_main(nullptr); // configure the class (required once per test executable)

    GIVEN("a default instance (no creation args: 2 in, 2 out, 4 branches)") {
        test_wrapper<ambitap_plate> an_instance;
        ambitap_plate&              my_object = an_instance;

        THEN("the tank decay and in-loop damping are the paper's table-1 values") {
            REQUIRE(static_cast<double>(my_object.decay) == 0.5);
            REQUIRE(static_cast<double>(my_object.damping) == 0.0005);
        }
        THEN("the input band-limiting one-pole is nearly wide open") {
            REQUIRE(static_cast<double>(my_object.bandwidth) == 0.9995);
        }
        THEN("diffusion is at the paper's unscaled allpass coefficients") {
            REQUIRE(static_cast<double>(my_object.diffusion) == 1.0);
        }
        THEN("there is no predelay") {
            REQUIRE(static_cast<double>(my_object.predelay) == 0.0);
        }
        THEN("the tank modulation is the paper's ~0.54 ms excursion at 1 Hz") {
            REQUIRE(static_cast<double>(my_object.moddepth) == 0.5376);
            REQUIRE(static_cast<double>(my_object.modrate) == 1.0);
        }
        THEN("the object is fully wet, matching the 100%-wet kernel") {
            REQUIRE(static_cast<double>(my_object.mix) == 1.0);
        }
    }
}

SCENARIO("ambitap.plate~ clamps the diffusion scale to 0..1.3") {
    ext_main(nullptr);

    GIVEN("a default instance") {
        test_wrapper<ambitap_plate> an_instance;
        ambitap_plate&              my_object = an_instance;

        WHEN("a diffusion above the documented ceiling is requested") {
            my_object.diffusion = 5.0;
            THEN("it clamps down to 1.3") {
                REQUIRE(static_cast<double>(my_object.diffusion) == 1.3);
            }
        }
        WHEN("a negative diffusion is requested") {
            my_object.diffusion = -1.0;
            THEN("it clamps up to 0") {
                REQUIRE(static_cast<double>(my_object.diffusion) == 0.0);
            }
        }
        WHEN("a diffusion inside the range is requested") {
            my_object.diffusion = 0.5;
            THEN("it is taken as-is") {
                REQUIRE(static_cast<double>(my_object.diffusion) == 0.5);
            }
        }
    }
}

SCENARIO("ambitap.plate~ clamps the dry/wet mix to 0..1") {
    ext_main(nullptr);

    GIVEN("a default instance") {
        test_wrapper<ambitap_plate> an_instance;
        ambitap_plate&              my_object = an_instance;

        WHEN("a mix above 1 is requested") {
            my_object.mix = 2.0;
            THEN("it clamps down to fully wet") {
                REQUIRE(static_cast<double>(my_object.mix) == 1.0);
            }
        }
        WHEN("a negative mix is requested") {
            my_object.mix = -0.5;
            THEN("it clamps up to fully dry") {
                REQUIRE(static_cast<double>(my_object.mix) == 0.0);
            }
        }
        WHEN("a mix inside the range is requested") {
            my_object.mix = 0.25;
            THEN("it is taken as-is") {
                REQUIRE(static_cast<double>(my_object.mix) == 0.25);
            }
        }
    }
}
