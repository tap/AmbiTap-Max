/// @file
/// @brief      Unit tests for ambitap.decode~ (Min-level: attribute defaults).
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include "c74_min_unittest.h" // required unit-test header (defines main via Catch)
// The object source is included second on purpose: min-api requires the
// Catch header to come first, and "ambitap." sorts ahead of it.
#include "ambitap.decode_tilde.cpp" // include the object source so we can instantiate it

SCENARIO("ambitap.decode~ instantiates with the documented defaults") {
    ext_main(nullptr); // configure the class (required once per test executable)

    GIVEN("a default instance (no creation args: order 1, stereo layout)") {
        test_wrapper<ambitap_decode> an_instance;
        ambitap_decode&              my_object = an_instance;

        THEN("the pseudoinverse (mode-matching) decoder is selected") {
            REQUIRE(my_object.decoder_type == symbol("mode_match"));
        }
        THEN("max-rE weighting is off, so the decode is basic (velocity) by default") {
            REQUIRE(static_cast<bool>(my_object.max_re) == false);
        }
    }
}

SCENARIO("ambitap.decode~ accepts the documented decoder algorithms") {
    ext_main(nullptr);

    GIVEN("a default instance") {
        test_wrapper<ambitap_decode> an_instance;
        ambitap_decode&              my_object = an_instance;

        WHEN("the AllRAD decoder is requested") {
            my_object.decoder_type = symbol("allrad");
            THEN("the attribute reports it") {
                REQUIRE(my_object.decoder_type == symbol("allrad"));
            }
        }
        WHEN("the energy-preserving decoder is requested") {
            my_object.decoder_type = symbol("epad");
            THEN("the attribute reports it") {
                REQUIRE(my_object.decoder_type == symbol("epad"));
            }
        }
        WHEN("max-rE weighting is switched on") {
            my_object.max_re = true;
            THEN("the attribute reports it") {
                REQUIRE(static_cast<bool>(my_object.max_re) == true);
            }
        }
    }
}
