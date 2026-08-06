/// @file
/// @brief      Unit tests for ambitap.encode~ (Min-level: attribute defaults, setter
///             forwarding and channel-count edges observed through the audio path).
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include "c74_min_unittest.h" // required unit-test header (defines main via Catch)
// The object source is included second on purpose: min-api requires the
// Catch header to come first, and "ambitap." sorts ahead of it.
#include "ambitap.encode_tilde.cpp" // include the object source so we can instantiate it

#include <vector>

namespace {

    constexpr double k_half_pi = 1.5707963267948966;

    // Drive the vector operator directly: mono input of `frames` ones into
    // `channels` output channels, returning the per-channel output of frame 0.
    // (Every frame is identical — the encoder is a static gain per channel.)
    std::vector<double> encode_ones(ambitap_encode& object, long channels, long frames = 4) {
        std::vector<double> in(frames, 1.0);
        double*             ins[] = {in.data()};

        std::vector<std::vector<double>> out(channels, std::vector<double>(frames, -99.0));
        std::vector<double*>             outs(channels);
        for (long ch = 0; ch < channels; ++ch) {
            outs[ch] = out[ch].data();
        }

        c74::min::audio_bundle input {ins, 1, frames};
        c74::min::audio_bundle output {outs.data(), channels, frames};
        object(input, output);

        std::vector<double> frame0(channels);
        for (long ch = 0; ch < channels; ++ch) {
            frame0[ch] = out[ch][0];
        }
        return frame0;
    }

} // namespace

SCENARIO("ambitap.encode~ instantiates with the documented defaults") {
    ext_main(nullptr); // configure the class (required once per test executable)

    GIVEN("a default instance (no creation args: order falls back to 1)") {
        test_wrapper<ambitap_encode> an_instance;
        ambitap_encode&              my_object = an_instance;

        THEN("the source sits dead ahead on the horizon") {
            REQUIRE(static_cast<double>(my_object.azimuth) == 0.0);
            REQUIRE(static_cast<double>(my_object.elevation) == 0.0);
        }
        THEN("the encoder is at unity gain") {
            REQUIRE(static_cast<double>(my_object.gain) == 1.0);
        }
        THEN("a front-facing source encodes as W and X only (AmbiX ACN/SN3D, order 1)") {
            auto frame = encode_ones(my_object, 4);
            REQUIRE(frame[0] == Approx(1.0));              // W
            REQUIRE(frame[1] == Approx(0.0).margin(1e-6)); // Y = sin(az)
            REQUIRE(frame[2] == Approx(0.0).margin(1e-6)); // Z = sin(el)
            REQUIRE(frame[3] == Approx(1.0));              // X = cos(az)cos(el)
        }
    }
}

SCENARIO("ambitap.encode~ forwards attribute changes to the encoder") {
    ext_main(nullptr);

    GIVEN("a default instance") {
        test_wrapper<ambitap_encode> an_instance;
        ambitap_encode&              my_object = an_instance;

        WHEN("the azimuth attribute moves the source to the left (pi/2)") {
            my_object.azimuth = k_half_pi;
            THEN("the energy moves from X to Y") {
                auto frame = encode_ones(my_object, 4);
                REQUIRE(frame[1] == Approx(1.0));              // Y = sin(pi/2)
                REQUIRE(frame[3] == Approx(0.0).margin(1e-6)); // X = cos(pi/2)
            }
        }
        WHEN("the gain attribute is halved") {
            my_object.gain = 0.5;
            THEN("every channel scales by the same factor") {
                auto frame = encode_ones(my_object, 4);
                REQUIRE(frame[0] == Approx(0.5)); // W
                REQUIRE(frame[3] == Approx(0.5)); // X
            }
        }
    }
}

SCENARIO("ambitap.encode~ zero-fills output channels beyond the encoder's bus") {
    ext_main(nullptr);

    GIVEN("a default order-1 instance (4-channel bus)") {
        test_wrapper<ambitap_encode> an_instance;
        ambitap_encode&              my_object = an_instance;

        WHEN("Max hands the operator more output channels than the bus carries") {
            auto frame = encode_ones(my_object, 6);
            THEN("the surplus channels are silent, not stale memory") {
                REQUIRE(frame[4] == 0.0);
                REQUIRE(frame[5] == 0.0);
            }
        }
    }
}
