/// @file
/// @brief      Unit tests for ambitap.mirror~ (Min-level: attribute defaults, setter
///             forwarding and channel-count edges observed through the audio path).
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include "c74_min_unittest.h" // required unit-test header (defines main via Catch)
// The object source is included second on purpose: min-api requires the
// Catch header to come first, and "ambitap." sorts ahead of it.
#include "ambitap.mirror_tilde.cpp" // include the object source so we can instantiate it

#include <vector>

namespace {

    // Drive the vector operator directly: `in_channels` inputs each carrying a distinct
    // constant (ch+1) into `out_channels` outputs, returning frame 0 per output channel.
    std::vector<double> mirror_frame(ambitap_mirror& object, long in_channels, long out_channels,
                                     long frames = 4) {
        std::vector<std::vector<double>> in(in_channels);
        std::vector<double*>             ins(in_channels);
        for (long ch = 0; ch < in_channels; ++ch) {
            in[ch].assign(frames, static_cast<double>(ch + 1));
            ins[ch] = in[ch].data();
        }

        std::vector<std::vector<double>> out(out_channels, std::vector<double>(frames, -99.0));
        std::vector<double*>             outs(out_channels);
        for (long ch = 0; ch < out_channels; ++ch) {
            outs[ch] = out[ch].data();
        }

        c74::min::audio_bundle input {ins.data(), in_channels, frames};
        c74::min::audio_bundle output {outs.data(), out_channels, frames};
        object(input, output);

        std::vector<double> frame0(out_channels);
        for (long ch = 0; ch < out_channels; ++ch) {
            frame0[ch] = out[ch][0];
        }
        return frame0;
    }

} // namespace

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
        THEN("the bus indeed passes through untouched") {
            auto frame = mirror_frame(my_object, 4, 4);
            REQUIRE(frame == std::vector<double> {1.0, 2.0, 3.0, 4.0});
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

SCENARIO("ambitap.mirror~ forwards flips to the kernel as per-channel SH signs") {
    ext_main(nullptr);

    GIVEN("a default order-1 instance (ACN: W, Y, Z, X)") {
        test_wrapper<ambitap_mirror> an_instance;
        ambitap_mirror&              my_object = an_instance;

        WHEN("flip_lr is on") {
            my_object.flip_lr = true;
            THEN("only Y (ACN 1) changes sign") {
                auto frame = mirror_frame(my_object, 4, 4);
                REQUIRE(frame == std::vector<double> {1.0, -2.0, 3.0, 4.0});
            }
        }
        WHEN("flip_ud is on") {
            my_object.flip_ud = true;
            THEN("only Z (ACN 2) changes sign") {
                auto frame = mirror_frame(my_object, 4, 4);
                REQUIRE(frame == std::vector<double> {1.0, 2.0, -3.0, 4.0});
            }
        }
        WHEN("flip_fb is on") {
            my_object.flip_fb = true;
            THEN("only X (ACN 3) changes sign") {
                auto frame = mirror_frame(my_object, 4, 4);
                REQUIRE(frame == std::vector<double> {1.0, 2.0, 3.0, -4.0});
            }
        }
        WHEN("the flip is turned back off") {
            my_object.flip_lr = true;
            my_object.flip_lr = false;
            THEN("the bus passes through untouched again") {
                auto frame = mirror_frame(my_object, 4, 4);
                REQUIRE(frame == std::vector<double> {1.0, 2.0, 3.0, 4.0});
            }
        }
    }
}

SCENARIO("ambitap.mirror~ zero-fills output channels it has no input for") {
    ext_main(nullptr);

    GIVEN("a default order-1 instance") {
        test_wrapper<ambitap_mirror> an_instance;
        ambitap_mirror&              my_object = an_instance;

        WHEN("the input bus carries fewer channels than the output") {
            auto frame = mirror_frame(my_object, 2, 4);
            THEN("the uncovered channels are silent, not stale memory") {
                REQUIRE(frame[0] == 1.0);
                REQUIRE(frame[1] == 2.0);
                REQUIRE(frame[2] == 0.0);
                REQUIRE(frame[3] == 0.0);
            }
        }
    }
}
