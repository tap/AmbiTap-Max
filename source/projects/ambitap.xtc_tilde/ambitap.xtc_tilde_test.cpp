/// @file
/// @brief      Unit tests for ambitap.xtc~ (Min-level: attribute defaults, clamping).
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include "c74_min_unittest.h" // required unit-test header (defines main via Catch)
// The object source is included second on purpose: min-api requires the
// Catch header to come first, and "ambitap." sorts ahead of it.
#include "ambitap.xtc_tilde.cpp" // include the object source so we can instantiate it

SCENARIO("ambitap.xtc~ instantiates with the verified desktop geometry") {
    ext_main(nullptr); // configure the class (required once per test executable)

    GIVEN("a default instance (no dsp yet, so no convolver quad is published)") {
        test_wrapper<ambitap_xtc> an_instance;
        ambitap_xtc&              my_object = an_instance;

        THEN("the speakers span the +/-10 degree desktop geometry the gates run at") {
            REQUIRE(static_cast<double>(my_object.span) == 20.0);
        }
        THEN("the stated listening distance is one meter") {
            REQUIRE(static_cast<double>(my_object.distance) == 1.0);
        }
        THEN("regularization is at the verified midpoint") {
            REQUIRE(static_cast<double>(my_object.regularization) == 0.5);
        }
        THEN("the A/B bypass leg starts disengaged") {
            REQUIRE(static_cast<bool>(my_object.bypass) == false);
        }
    }
}

SCENARIO("ambitap.xtc~ clamps the speaker span to the designer's 5..120 degree range") {
    ext_main(nullptr);

    GIVEN("a default instance") {
        test_wrapper<ambitap_xtc> an_instance;
        ambitap_xtc&              my_object = an_instance;

        WHEN("a span wider than the design range is requested") {
            my_object.span = 200.0;
            THEN("it clamps down to xtc::k_max_span_deg") {
                REQUIRE(static_cast<double>(my_object.span) == 120.0);
            }
        }
        WHEN("a span narrower than the design range is requested") {
            my_object.span = 1.0;
            THEN("it clamps up to xtc::k_min_span_deg") {
                REQUIRE(static_cast<double>(my_object.span) == 5.0);
            }
        }
        WHEN("a span inside the range is requested") {
            my_object.span = 60.0;
            THEN("it is taken as-is") {
                REQUIRE(static_cast<double>(my_object.span) == 60.0);
            }
        }
    }
}

SCENARIO("ambitap.xtc~ clamps the listening distance to the designer's floor") {
    ext_main(nullptr);

    GIVEN("a default instance") {
        test_wrapper<ambitap_xtc> an_instance;
        ambitap_xtc&              my_object = an_instance;

        WHEN("a distance below the floor is requested") {
            my_object.distance = 0.05;
            THEN("it clamps up to xtc::k_min_distance (0.1 m, exact as the float the "
                 "designer round-trips it through)") {
                REQUIRE(static_cast<double>(my_object.distance) == static_cast<double>(0.1f));
            }
        }
        WHEN("a plausible room distance is requested") {
            my_object.distance = 2.0;
            THEN("it is taken as-is") {
                REQUIRE(static_cast<double>(my_object.distance) == 2.0);
            }
        }
    }
}

SCENARIO("ambitap.xtc~ clamps the regularization amount to 0..1") {
    ext_main(nullptr);

    GIVEN("a default instance") {
        test_wrapper<ambitap_xtc> an_instance;
        ambitap_xtc&              my_object = an_instance;

        WHEN("an amount above the documented range is requested") {
            my_object.regularization = 5.0;
            THEN("it clamps down to 1") {
                REQUIRE(static_cast<double>(my_object.regularization) == 1.0);
            }
        }
        WHEN("a negative amount is requested") {
            my_object.regularization = -1.0;
            THEN("it clamps up to 0") {
                REQUIRE(static_cast<double>(my_object.regularization) == 0.0);
            }
        }
    }
}
