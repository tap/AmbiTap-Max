/// @file
/// ambitap.rotate~ — rotate a higher-order ambisonics bus by yaw/pitch/roll
/// (Euler angles, applied Z-Y-X; AmbiX: ACN/SN3D).
///
/// Order is a creation argument (default 1). Input and output are each a single
/// multichannel signal of (order+1)^2 channels. DSP lives in
/// ambitap::dsp::rotator, whose SH-rotation matrix is rebuilt off the audio
/// thread (async_rebuilder worker) and read wait-free here.
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include <algorithm>
#include <memory>
#include <vector>

#include "ambitap/dsp/rotator.h"
#include "c74_min.h"

using namespace c74::min;

class ambitap_rotate : public object<ambitap_rotate>, public mc_operator<> {
  public:
    MIN_DESCRIPTION{"Rotate a higher-order ambisonics bus by yaw/pitch/roll (Euler, Z-Y-X). "
                    "Order is a creation argument; input and output are one multichannel "
                    "signal of (order+1)^2 channels."};
    MIN_TAGS{"audio, ambisonics, spatial, mc"};
    MIN_AUTHOR{"Timothy Place"};
    MIN_RELATED{"ambitap.encode~, ambitap.decode~, ambitap.binaural~"};

    inlet<>  m_in{this, "(multichannelsignal) HOA bus"};
    outlet<> m_out{this, "(multichannelsignal) rotated HOA bus, (order+1)^2 channels", "signal"};

  private:
    // State lives ABOVE the attributes on purpose: min-api attribute
    // construction invokes the custom setter with the default value, and
    // members are initialized in declaration order — everything a setter
    // touches must already be alive.
    std::unique_ptr<ambitap::dsp::rotator> m_rotator;
    long                                   m_channel_count{4};
    std::vector<float>                     m_in_frame; // sized once, reused (RT-safe)
    std::vector<float>                     m_out_frame;

  public:
    /// First creation argument is the ambisonics order (default 1).
    explicit ambitap_rotate(const atoms& args = {}) {
        int order = 1;
        if (!args.empty()) {
            order = std::clamp(static_cast<int>(args[0]), 0, ambitap::k_max_order);
        }
        m_rotator       = std::make_unique<ambitap::dsp::rotator>(order);
        m_channel_count = static_cast<long>(m_rotator->channels());
        m_in_frame.assign(static_cast<size_t>(m_channel_count), 0.0f);
        m_out_frame.assign(static_cast<size_t>(m_channel_count), 0.0f);
    }

    attribute<number> yaw{this, "yaw", 0.0, description{"Rotation about +Z (up), radians. Applied first."},
                          setter{MIN_FUNCTION{const double v = args[0];
    if (m_rotator) {
        m_rotator->set_yaw(static_cast<float>(v));
    }
    return {v};
}
}
}
;

attribute<number> pitch{this, "pitch", 0.0, description{"Rotation about +Y (left), radians. Applied second."},
                        setter{MIN_FUNCTION{const double v = args[0];
if (m_rotator) {
    m_rotator->set_pitch(static_cast<float>(v));
}
return {v};
}
}
}
;

attribute<number> roll{this, "roll", 0.0, description{"Rotation about +X (front), radians. Applied last."},
                       setter{MIN_FUNCTION{const double v = args[0];
if (m_rotator) {
    m_rotator->set_roll(static_cast<float>(v));
}
return {v};
}
}
}
;

/// Register Max's multichanneloutputs so the output reports (order+1)^2
/// channels. (min-api wraps MC inlets, but not MC outputs.)
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
    const long n      = m_channel_count;

    for (auto i = 0; i < frames; ++i) {
        // Gather this frame's HOA channels (double -> float, zero-padded).
        for (long c = 0; c < n; ++c) {
            m_in_frame[c] = (c < in_ch) ? static_cast<float>(input.samples(c)[i]) : 0.0f;
        }

        // Wait-free rotate (identity passthrough until the first matrix lands).
        m_rotator->process_frame(m_in_frame.data(), m_out_frame.data());

        // Scatter back to the multichannel output (float -> double).
        for (long c = 0; c < out_ch; ++c) {
            output.samples(c)[i] = (c < n) ? static_cast<double>(m_out_frame[c]) : 0.0;
        }
    }
}

private:
static long mc_outputs(minwrap<ambitap_rotate>* self, long /* outlet_index */) {
    return self->m_min_object.m_channel_count;
}
}
;

MIN_EXTERNAL(ambitap_rotate);
