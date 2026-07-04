/// @file
/// ambitap.distance~ — distance cues for a higher-order ambisonics bus:
/// propagation delay / Doppler, 1/r^n distance gain, air-absorption low-pass,
/// and near-field compensation (NFC-HOA, Daniel's per-order shelving).
/// Order is a creation argument; multichannel in and out.
///
/// Processing chain (per frame): doppler delay -> distance gain -> air
/// absorption -> NFC. The delay comes first because it models propagation of
/// the dry bus — modulating 'distance' then yields the physical pitch glide
/// before any level/tone shaping tied to the current distance. Gain and the
/// air-absorption one-pole are applied uniformly across channels (spatial
/// encoding preserved), and the per-order NFC shelf runs last so its bass
/// correction is not fed through the time-varying delay interpolation.
///
/// Air absorption model: a single one-pole low-pass shared by all channels,
/// with cutoff falling with the excess distance beyond the reference:
///     fc(d) = 20 kHz / (1 + amount * 0.1 per-meter * max(d - d_ref, 0))
/// i.e. at amount 1.0 a source 90 m beyond the reference is filtered at
/// 2 kHz. Simple, standard, and monotonic — not the full ISO 9613 curve.

#include "c74_min.h"

#include "ambitap/dsp/doppler.h"
#include "ambitap/dsp/nfc.h"

#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>

using namespace c74::min;

class ambitap_distance : public object<ambitap_distance>, public mc_operator<> {
public:
    MIN_DESCRIPTION {"Distance cues for a higher-order ambisonics bus: Doppler delay, "
                     "1/r gain, air-absorption low-pass, and near-field compensation "
                     "(NFC-HOA). Order is a creation arg."};
    MIN_TAGS {"audio, ambisonics, spatial, distance, doppler, nfc, mc"};
    MIN_AUTHOR {"Timothy Place"};
    MIN_RELATED {"ambitap.doppler~, ambitap.encode~, ambitap.decode~"};

    inlet<>  m_in {this, "(multichannelsignal) HOA bus"};
    outlet<> m_out {this, "(multichannelsignal) HOA bus with distance cues", "signal"};

private:
    // State lives ABOVE the attributes on purpose: min-api attribute
    // construction invokes the custom setter with the default value, and
    // members are initialized in declaration order — everything a setter
    // touches must already be alive.

    /// One-pole coefficient of the per-sample distance slew for the gain and
    /// air-absorption cues (matches dsp::doppler's delay slew).
    static constexpr float k_distance_slew = 1.0f / 1024.0f;

    std::unique_ptr<ambitap::dsp::doppler> m_doppler;
    std::unique_ptr<ambitap::dsp::nfc>     m_nfc;
    long                                   m_channel_count {4};
    float                                  m_fs {48000.0f};
    float                                  m_distance_smooth {1.0f};
    std::vector<float>                     m_frame;
    std::vector<float>                     m_lp_state;

public:
    explicit ambitap_distance(const atoms& args = {}) {
        int order = 1;
        if (!args.empty())
            order = std::clamp(static_cast<int>(args[0]), 1, ambitap::max_order);
        m_doppler       = std::make_unique<ambitap::dsp::doppler>(order);
        m_nfc           = std::make_unique<ambitap::dsp::nfc>(order);
        m_channel_count = static_cast<long>(m_nfc->channels());
        m_frame.assign(static_cast<size_t>(m_channel_count), 0.0f);
        m_lp_state.assign(static_cast<size_t>(m_channel_count), 0.0f);
    }

    attribute<number> distance {
        this, "distance", 1.0,
        description {"Source-to-listener distance in meters. Drives all four cues; "
                     "modulate for Doppler."},
        setter {MIN_FUNCTION {
            const double v = args[0];
            if (m_doppler)
                m_doppler->set_distance(static_cast<float>(v));
            if (m_nfc)
                m_nfc->set_source_distance(static_cast<float>(v));
            return {v};
        }}
    };

    attribute<number> reference_distance {
        this, "reference_distance", 1.0,
        description {"Reference distance in meters (loudspeaker/decoder radius): unity gain, "
                     "no air absorption, and NFC identity at this distance."},
        setter {MIN_FUNCTION {
            const double v = args[0];
            if (m_nfc)
                m_nfc->set_reference_distance(static_cast<float>(v));
            return {v};
        }}
    };

    attribute<number> attenuation {
        this, "attenuation", 1.0,
        description {"Distance-gain exponent: gain = (reference_distance / distance)^attenuation. "
                     "1 is the physical 1/r law; 0 disables distance gain. Distance is clamped "
                     "to a 0.1 m minimum."},
        setter {MIN_FUNCTION {
            const double v = args[0];
            return {std::max(v, 0.0)};
        }}
    };

    attribute<number> air_absorption {
        this, "air_absorption", 0.0,
        description {"Air-absorption amount, 0..1: distance-dependent one-pole low-pass "
                     "(see source header for the cutoff model). 0 disables it."},
        setter {MIN_FUNCTION {
            const double v = args[0];
            return {std::clamp(v, 0.0, 1.0)};
        }}
    };

    attribute<number> speed_of_sound {
        this, "speed_of_sound", 343.0,
        description {"Speed of sound in m/s (delay time-of-flight and NFC corner scaling)."},
        setter {MIN_FUNCTION {
            const double v = args[0];
            if (m_doppler)
                m_doppler->set_speed_of_sound(static_cast<float>(v));
            if (m_nfc)
                m_nfc->set_speed_of_sound(static_cast<float>(v));
            return {v};
        }}
    };

    attribute<number> max_distance {
        this, "max_distance", 50.0,
        description {"Maximum distance (sizes the Doppler delay buffer). Reallocates on change."},
        setter {MIN_FUNCTION {
            const double v = args[0];
            if (m_doppler)
                m_doppler->set_max_distance(static_cast<float>(v));
            return {v};
        }}
    };

    attribute<bool> doppler_enabled {
        this, "doppler", true,
        description {"Enable the propagation delay (Doppler on distance modulation)."}
    };

    attribute<bool> nfc_enabled {
        this, "nfc", true,
        description {"Enable near-field compensation (per-order NFC-HOA bass shelving)."}
    };

    /// The delay buffers are sized from the host sample rate.
    message<> dspsetup {
        this, "dspsetup", MIN_FUNCTION {
            const double sr = args[0];
            m_fs            = static_cast<float>(sr);
            if (m_doppler)
                m_doppler->prepare(m_fs);
            if (m_nfc)
                m_nfc->prepare(m_fs);
            std::fill(m_lp_state.begin(), m_lp_state.end(), 0.0f);
            m_distance_smooth = static_cast<float>(static_cast<double>(distance));
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
        const auto  frames = input.frame_count();
        const auto  in_ch  = input.channel_count();
        const auto  out_ch = output.channel_count();
        const long  n      = m_channel_count;
        const float d_min  = ambitap::dsp::nfc::k_min_distance;

        const float d_target = std::max(static_cast<float>(static_cast<double>(distance)), d_min);
        const float d_ref    = std::max(
            static_cast<float>(static_cast<double>(reference_distance)), d_min);
        const float exponent = static_cast<float>(static_cast<double>(attenuation));
        const float air      = static_cast<float>(static_cast<double>(air_absorption));
        const bool  dop_on   = doppler_enabled;
        const bool  nfc_on   = nfc_enabled;

        for (auto i = 0; i < frames; ++i) {
            for (long c = 0; c < n; ++c)
                m_frame[c] = (c < in_ch) ? static_cast<float>(input.samples(c)[i]) : 0.0f;

            // 1. Propagation delay (Doppler). The library slews the delay
            //    internally, producing the pitch glide on distance jumps.
            if (dop_on)
                m_doppler->process_frame(m_frame.data(), m_frame.data());

            // Smooth the distance for the gain/absorption cues (same slew
            // constant as the doppler delay: ~1024 samples).
            m_distance_smooth += (d_target - m_distance_smooth) * k_distance_slew;
            const float d = std::max(m_distance_smooth, d_min);

            // 2. Distance gain: (d_ref / d)^attenuation, uniform across
            //    channels so the spatial encoding is preserved.
            const float gain = (exponent > 0.0f) ? std::pow(d_ref / d, exponent) : 1.0f;

            // 3. Air absorption: shared one-pole low-pass, cutoff falling
            //    with excess distance beyond the reference (model above).
            float lp_coeff = 0.0f;
            if (air > 0.0f) {
                const float excess = std::max(d - d_ref, 0.0f);
                const float fc     = 20000.0f / (1.0f + air * 0.1f * excess);
                lp_coeff           = 1.0f - std::exp(-6.2831853f * fc / m_fs);
                lp_coeff           = std::min(lp_coeff, 1.0f);
            }
            for (long c = 0; c < n; ++c) {
                float v = m_frame[c] * gain;
                if (air > 0.0f) {
                    auto& s = m_lp_state[static_cast<size_t>(c)];
                    s += lp_coeff * (v - s);
                    v = s;
                }
                m_frame[c] = v;
            }

            // 4. Near-field compensation: per-order bass shelf referencing
            //    the decoder radius (identity when d == d_ref).
            if (nfc_on)
                m_nfc->process_frame(m_frame.data(), m_frame.data());

            for (long c = 0; c < out_ch; ++c)
                output.samples(c)[i] = (c < n) ? static_cast<double>(m_frame[c]) : 0.0;
        }
    }

private:
    static long mc_outputs(minwrap<ambitap_distance>* self, long) {
        return self->m_min_object.m_channel_count;
    }
};

MIN_EXTERNAL(ambitap_distance);
