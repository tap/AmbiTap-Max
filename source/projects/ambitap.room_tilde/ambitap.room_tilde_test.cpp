/// @file
/// @brief      Unit tests for ambitap.room~ (Min-level: attribute defaults, clamping, coercion).
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include "c74_min_unittest.h" // required unit-test header (defines main via Catch)
// The object source is included second on purpose: min-api requires the
// Catch header to come first, and "ambitap." sorts ahead of it.
#include "ambitap.room_tilde.cpp" // include the object source so we can instantiate it

SCENARIO("ambitap.room~ instantiates with the library's verified seed-11 geometry") {
    ext_main(nullptr); // configure the class (required once per test executable)

    GIVEN("a default instance (no creation args: order falls back to 1)") {
        test_wrapper<ambitap_room> an_instance;
        ambitap_room&              my_object = an_instance;

        THEN("the shoebox is the documented 7.10 x 5.30 x 3.10 m room") {
            REQUIRE(static_cast<double>(my_object.dim_x) == 7.10);
            REQUIRE(static_cast<double>(my_object.dim_y) == 5.30);
            REQUIRE(static_cast<double>(my_object.dim_z) == 3.10);
        }
        THEN("the source sits at the verified off-axis position") {
            REQUIRE(static_cast<double>(my_object.source_x) == 3.674);
            REQUIRE(static_cast<double>(my_object.source_y) == 1.137);
            REQUIRE(static_cast<double>(my_object.source_z) == 1.977);
        }
        THEN("the listener sits at the verified off-axis position") {
            REQUIRE(static_cast<double>(my_object.listener_x) == 1.746);
            REQUIRE(static_cast<double>(my_object.listener_y) == 1.711);
            REQUIRE(static_cast<double>(my_object.listener_z) == 0.668);
        }
        THEN("the broadband reverb time is 0.76 s") {
            REQUIRE(static_cast<double>(my_object.rt60) == 0.76);
        }
        THEN("all three paths — direct, early reflections, and FDN tail — are enabled") {
            REQUIRE(static_cast<bool>(my_object.direct) == true);
            REQUIRE(static_cast<bool>(my_object.er) == true);
            REQUIRE(static_cast<bool>(my_object.tail) == true);
        }
        THEN("the output is at unity gain") {
            REQUIRE(static_cast<double>(my_object.gain) == 1.0);
        }
        THEN("loop absorption uses the verified 255-tap linear-phase FIRs") {
            REQUIRE(my_object.absorption == symbol("fir"));
        }
    }
}

SCENARIO("ambitap.room~ clamps the output gain to non-negative") {
    ext_main(nullptr);

    GIVEN("a default instance") {
        test_wrapper<ambitap_room> an_instance;
        ambitap_room&              my_object = an_instance;

        WHEN("a negative gain is requested") {
            my_object.gain = -2.0;
            THEN("it clamps to silence rather than inverting the bus") {
                REQUIRE(static_cast<double>(my_object.gain) == 0.0);
            }
        }
        WHEN("a boost is requested") {
            my_object.gain = 4.0;
            THEN("it is taken as-is (there is no upper bound)") {
                REQUIRE(static_cast<double>(my_object.gain) == 4.0);
            }
        }
    }
}

SCENARIO("ambitap.room~ coerces the absorption kind to one of the two known filters") {
    ext_main(nullptr);

    GIVEN("a default instance") {
        test_wrapper<ambitap_room> an_instance;
        ambitap_room&              my_object = an_instance;

        WHEN("the cheap first-order IIR absorption is requested") {
            my_object.absorption = symbol("iir");
            THEN("the attribute reports it") {
                REQUIRE(my_object.absorption == symbol("iir"));
            }
        }
        WHEN("an unknown filter name is requested after switching to IIR") {
            my_object.absorption = symbol("iir");
            my_object.absorption = symbol("bogus");
            THEN("the setter falls back to the default FIR rather than storing the name") {
                REQUIRE(my_object.absorption == symbol("fir"));
            }
        }
    }
}
