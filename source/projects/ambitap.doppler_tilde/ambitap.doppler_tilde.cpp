/// @file
/// ambitap.doppler~ — variable propagation delay for a higher-order ambisonics
/// bus (distance-based time-of-flight; modulate distance for Doppler shift).
/// Order is a creation argument; multichannel in and out.

#include "c74_min.h"

#include "ambitap/dsp/doppler.h"

#include <algorithm>
#include <memory>
#include <vector>

using namespace c74::min;

class ambitap_doppler : public object<ambitap_doppler>, public mc_operator<> {
public:
    MIN_DESCRIPTION {"Variable propagation delay for a higher-order ambisonics bus. "
                     "Modulate 'distance' for the Doppler effect. Order is a creation arg."};
    MIN_TAGS {"audio, ambisonics, spatial, delay, doppler, mc"};
    MIN_AUTHOR {"Timothy Place"};
    MIN_RELATED {"ambitap.encode~, delay~"};

    inlet<>  m_in {this, "(multichannelsignal) HOA bus"};
    outlet<> m_out {this, "(multichannelsignal) delayed HOA bus", "signal"};

private:
    // State lives ABOVE the attributes on purpose: min-api attribute
    // construction invokes the custom setter with the default value, and
    // members are initialized in declaration order — everything a setter
    // touches must already be alive.
    std::unique_ptr<ambitap::dsp::doppler> m_doppler;
    long                                   m_channel_count {4};
    std::vector<float>                     m_in_frame;
    std::vector<float>                     m_out_frame;

public:
    explicit ambitap_doppler(const atoms& args = {}) {
        int order = 1;
        if (!args.empty())
            order = std::clamp(static_cast<int>(args[0]), 1, ambitap::max_order);
        m_doppler       = std::make_unique<ambitap::dsp::doppler>(order);
        m_channel_count = static_cast<long>(m_doppler->channels());
        m_in_frame.assign(static_cast<size_t>(m_channel_count), 0.0f);
        m_out_frame.assign(static_cast<size_t>(m_channel_count), 0.0f);
    }

    attribute<number> distance {
        this, "distance", 1.0,
        description {"Source-to-listener distance in meters. Modulate for Doppler."},
        setter {MIN_FUNCTION {
            const double v = args[0];
            if (m_doppler)
                m_doppler->set_distance(static_cast<float>(v));
            return {v};
        }}
    };

    attribute<number> speed_of_sound {
        this, "speed_of_sound", 343.0,
        description {"Speed of sound in m/s."},
        setter {MIN_FUNCTION {
            const double v = args[0];
            if (m_doppler)
                m_doppler->set_speed_of_sound(static_cast<float>(v));
            return {v};
        }}
    };

    attribute<number> max_distance {
        this, "max_distance", 50.0,
        description {"Maximum distance (sizes the delay buffer). Reallocates on change."},
        setter {MIN_FUNCTION {
            const double v = args[0];
            if (m_doppler)
                m_doppler->set_max_distance(static_cast<float>(v));
            return {v};
        }}
    };

    /// The delay buffers are sized from the host sample rate.
    message<> dspsetup {
        this, "dspsetup", MIN_FUNCTION {
            const double sr = args[0];
            if (m_doppler)
                m_doppler->prepare(static_cast<float>(sr));
            return {};
        }
    };

    message<> maxclass_setup {
        this, "maxclass_setup", MIN_FUNCTION {
            c74::max::t_class* c = args[0];
            c74::max::class_addmethod(c, reinterpret_cast<c74::max::method>(mc_outputs),
                                      "multichanneloutputs", c74::max::A_CANT, 0);
            return {};
        }
    };

    void operator()(audio_bundle input, audio_bundle output) {
        const auto frames = input.frame_count();
        const auto in_ch  = input.channel_count();
        const auto out_ch = output.channel_count();
        const long n      = m_channel_count;

        for (auto i = 0; i < frames; ++i) {
            for (long c = 0; c < n; ++c)
                m_in_frame[c] = (c < in_ch) ? static_cast<float>(input.samples(c)[i]) : 0.0f;
            m_doppler->process_frame(m_in_frame.data(), m_out_frame.data());
            for (long c = 0; c < out_ch; ++c)
                output.samples(c)[i] = (c < n) ? static_cast<double>(m_out_frame[c]) : 0.0;
        }
    }

private:
    static long mc_outputs(minwrap<ambitap_doppler>* self, long) {
        return self->m_min_object.m_channel_count;
    }
};

MIN_EXTERNAL(ambitap_doppler);
