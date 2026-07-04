/// @file
/// ambitap.energyvec~ — active-intensity (broadband energy vector) DOA estimate
/// for a higher-order ambisonics bus. Uses the first four channels (W, Y, Z, X);
/// outputs the smoothed intensity vector as three signals (x, y, z).

#include "c74_min.h"

#include "ambitap/analysis/energy_vector.h"

#include <array>

using namespace c74::min;

class ambitap_energyvec : public object<ambitap_energyvec>, public mc_operator<> {
public:
    MIN_DESCRIPTION {"Active-intensity (energy vector) DOA estimate for a HOA bus. Uses the "
                     "first four channels (W, Y, Z, X); outputs the smoothed x / y / z "
                     "intensity as three signals."};
    MIN_TAGS {"audio, ambisonics, spatial, analysis, mc"};
    MIN_AUTHOR {"Timothy Place"};
    MIN_RELATED {"ambitap.vmic~, ambitap.directional~"};

    inlet<>  m_in {this, "(multichannelsignal) HOA bus (>= order 1)"};
    outlet<> m_x {this, "(signal) X intensity (front +)", "signal"};
    outlet<> m_y {this, "(signal) Y intensity (left +)", "signal"};
    outlet<> m_z {this, "(signal) Z intensity (up +)", "signal"};

private:
    // State lives ABOVE the attributes on purpose: min-api attribute
    // construction invokes the custom setter with the default value, and
    // members are initialized in declaration order — everything a setter
    // touches must already be alive.
    ambitap::analysis::energy_vector m_estimator;

public:
    attribute<number> smoothing_time {
        this, "smoothing_time", 0.01,
        description {"One-pole smoothing time constant in seconds (~10 ms is usable)."},
        setter {MIN_FUNCTION {
            const double v = args[0];
            m_estimator.set_smoothing_time(static_cast<float>(v));
            return {v};
        }}
    };

    /// Smoothing coefficient depends on the host sample rate.
    message<> dspsetup {
        this, "dspsetup", MIN_FUNCTION {
            const double sr = args[0];
            m_estimator.prepare(static_cast<float>(sr));
            return {};
        }
    };

    void operator()(audio_bundle input, audio_bundle output) {
        const auto frames = input.frame_count();
        const auto in_ch  = input.channel_count();
        double*    ox      = output.samples(0);
        double*    oy      = output.samples(1);
        double*    oz      = output.samples(2);

        std::array<float, 4> in4 {};
        float                out3[3] {};

        for (auto i = 0; i < frames; ++i) {
            for (long c = 0; c < 4; ++c)
                in4[static_cast<size_t>(c)] = (c < in_ch) ? static_cast<float>(input.samples(c)[i]) : 0.0f;
            m_estimator.process_frame(in4.data(), out3);
            ox[i] = static_cast<double>(out3[0]);
            oy[i] = static_cast<double>(out3[1]);
            oz[i] = static_cast<double>(out3[2]);
        }
    }
};

MIN_EXTERNAL(ambitap_energyvec);
