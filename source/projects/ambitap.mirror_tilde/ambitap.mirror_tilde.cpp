/// @file
/// ambitap.mirror~ — mirror a higher-order ambisonics bus across cardinal
/// planes (left-right, front-back, up-down) via per-channel SH sign flips.
/// Order is a creation argument; multichannel in and out.
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include <algorithm>
#include <memory>

#include "ambitap/dsp/mirror.h"
#include "c74_min.h"

using namespace c74::min;

class ambitap_mirror : public object<ambitap_mirror>, public mc_operator<> {
  public:
    MIN_DESCRIPTION{"Mirror a higher-order ambisonics bus across cardinal planes "
                    "(left-right / front-back / up-down). Order is a creation argument."};
    MIN_TAGS{"audio, ambisonics, spatial, mc"};
    MIN_AUTHOR{"Timothy Place"};
    MIN_RELATED{"ambitap.rotate~, ambitap.encode~, ambitap.decode~"};

    inlet<>  m_in{this, "(multichannelsignal) HOA bus"};
    outlet<> m_out{this, "(multichannelsignal) mirrored HOA bus", "signal"};

  private:
    // State lives ABOVE the attributes on purpose: min-api attribute
    // construction invokes the custom setter with the default value, and
    // members are initialized in declaration order — everything a setter
    // touches must already be alive.
    std::unique_ptr<ambitap::dsp::mirror> m_mirror;
    long                                  m_channel_count{4};

  public:
    explicit ambitap_mirror(const atoms& args = {}) {
        int order = 1;
        if (!args.empty()) {
            order = std::clamp(static_cast<int>(args[0]), 0, ambitap::k_max_order);
        }
        m_mirror        = std::make_unique<ambitap::dsp::mirror>(order);
        m_channel_count = static_cast<long>(m_mirror->channels());
    }

    attribute<bool> flip_lr{this, "flip_lr", false, description{"Mirror left/right."},
                            setter{MIN_FUNCTION{const bool on = args[0];
    if (m_mirror) {
        m_mirror->set_flip_lr(on);
    }
    return {on};
}
}
}
;

attribute<bool> flip_fb{this, "flip_fb", false, description{"Mirror front/back."},
                        setter{MIN_FUNCTION{const bool on = args[0];
if (m_mirror) {
    m_mirror->set_flip_fb(on);
}
return {on};
}
}
}
;

attribute<bool> flip_ud{this, "flip_ud", false, description{"Mirror up/down."},
                        setter{MIN_FUNCTION{const bool on = args[0];
if (m_mirror) {
    m_mirror->set_flip_ud(on);
}
return {on};
}
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

    for (long c = 0; c < out_ch; ++c) {
        double*      o    = output.samples(c);
        const double sign = (c < m_channel_count) ? m_mirror->channel_sign(c) : 0.0;
        if (c < in_ch) {
            const double* s = input.samples(c);
            for (auto i = 0; i < frames; ++i) {
                o[i] = s[i] * sign;
            }
        }
        else {
            for (auto i = 0; i < frames; ++i) {
                o[i] = 0.0;
            }
        }
    }
}

private:
static long mc_outputs(minwrap<ambitap_mirror>* self, long) {
    return self->m_min_object.m_channel_count;
}
}
;

MIN_EXTERNAL(ambitap_mirror);
