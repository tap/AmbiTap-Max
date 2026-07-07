/// @file
/// ambitap.compress~ — spatial-image-preserving compressor for a higher-order
/// ambisonics bus: the detector keys off W (omni), and the same gain is applied
/// to every channel, so the directional image is preserved. Order is a creation
/// argument; multichannel in and out.
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include <algorithm>
#include <memory>

#include "ambitap/dsp/spatial_compressor.h"
#include "c74_min.h"

using namespace c74::min;

class ambitap_compress : public object<ambitap_compress>, public mc_operator<> {
  public:
    MIN_DESCRIPTION{"Spatial-image-preserving compressor for a HOA bus: detector on W, "
                    "uniform gain applied to all channels. Order is a creation argument."};
    MIN_TAGS{"audio, ambisonics, spatial, dynamics, mc"};
    MIN_AUTHOR{"Timothy Place"};
    MIN_RELATED{"ambitap.encode~, compressor~"};

    inlet<>  m_in{this, "(multichannelsignal) HOA bus"};
    outlet<> m_out{this, "(multichannelsignal) compressed HOA bus", "signal"};

  private:
    // State lives ABOVE the attributes on purpose: min-api attribute
    // construction invokes the custom setter with the default value, and
    // members are initialized in declaration order — everything a setter
    // touches must already be alive.
    std::unique_ptr<ambitap::dsp::spatial_compressor> m_compressor;
    long                                              m_channel_count{4};

  public:
    explicit ambitap_compress(const atoms& args = {}) {
        int order = 1;
        if (!args.empty()) {
            order = std::clamp(static_cast<int>(args[0]), 1, ambitap::k_max_order);
        }
        m_compressor    = std::make_unique<ambitap::dsp::spatial_compressor>(order);
        m_channel_count = static_cast<long>(m_compressor->channels());
    }

    attribute<number> threshold{this, "threshold", -12.0, description{"Compression threshold in dB."},
                                setter{MIN_FUNCTION{const double v = args[0];
    if (m_compressor) {
        m_compressor->set_threshold_db(static_cast<float>(v));
    }
    return {v};
}
}
}
;

attribute<number> ratio{this, "ratio", 4.0, description{"Compression ratio. 1 = no compression."},
                        setter{MIN_FUNCTION{const double v = args[0];
if (m_compressor) {
    m_compressor->set_ratio(static_cast<float>(v));
}
return {v};
}
}
}
;

attribute<number> attack{this, "attack", 0.005, description{"Attack time in seconds."},
                         setter{MIN_FUNCTION{const double v = args[0];
if (m_compressor) {
    m_compressor->set_attack(static_cast<float>(v));
}
return {v};
}
}
}
;

attribute<number> release{this, "release", 0.1, description{"Release time in seconds."},
                          setter{MIN_FUNCTION{const double v = args[0];
if (m_compressor) {
    m_compressor->set_release(static_cast<float>(v));
}
return {v};
}
}
}
;

attribute<number> makeup_gain{this, "makeup_gain", 0.0, description{"Makeup gain in dB."},
                              setter{MIN_FUNCTION{const double v = args[0];
if (m_compressor) {
    m_compressor->set_makeup_gain_db(static_cast<float>(v));
}
return {v};
}
}
}
;

/// Envelope coefficients depend on the host sample rate.
message<> dspsetup{this, "dspsetup", MIN_FUNCTION{const double sr = args[0];
if (m_compressor) {
    m_compressor->prepare(static_cast<float>(sr));
}
return {};
}
}
;

message<> maxclass_setup{this, "maxclass_setup", MIN_FUNCTION{c74::max::t_class* c = args[0];
c74::max::class_addmethod(c, reinterpret_cast<c74::max::method>(mc_outputs), "multichanneloutputs", c74::max::A_CANT,
                          0);
return {};
}
}
;

void operator()(audio_bundle input, audio_bundle output) {
    const auto frames = input.frame_count();
    const auto in_ch  = input.channel_count();
    const auto out_ch = output.channel_count();

    for (auto i = 0; i < frames; ++i) {
        // Detector keys off W (channel 0); same gain to every channel.
        const float  w    = (in_ch > 0) ? static_cast<float>(input.samples(0)[i]) : 0.0f;
        const double gain = m_compressor->process_envelope(w);
        for (long c = 0; c < out_ch; ++c) {
            output.samples(c)[i] = (c < in_ch) ? input.samples(c)[i] * gain : 0.0;
        }
    }
}

private:
static long mc_outputs(minwrap<ambitap_compress>* self, long) {
    return self->m_min_object.m_channel_count;
}
}
;

MIN_EXTERNAL(ambitap_compress);
