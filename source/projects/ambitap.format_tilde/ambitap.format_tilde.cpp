/// @file
/// ambitap.format~ — convert an ambisonics bus between FuMa (legacy B-format)
/// and AmbiX (ACN ordering + SN3D). Orders 0-3; order is a creation argument.
/// Multichannel in and out (channel permutation + per-channel gain).
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include <algorithm>
#include <memory>
#include <string>

#include "ambitap/dsp/format_converter.h"
#include "c74_min.h"

using namespace c74::min;

class ambitap_format : public object<ambitap_format>, public mc_operator<> {
  public:
    MIN_DESCRIPTION{"Convert an ambisonics bus between FuMa and AmbiX (ACN/SN3D). "
                    "Orders 0-3; order is a creation argument."};
    MIN_TAGS{"audio, ambisonics, format, fuma, ambix, mc"};
    MIN_AUTHOR{"Timothy Place"};
    MIN_RELATED{"ambitap.encode~, ambitap.decode~"};

    inlet<>  m_in{this, "(multichannelsignal) ambisonics bus"};
    outlet<> m_out{this, "(multichannelsignal) converted bus", "signal"};

  private:
    // State lives ABOVE the attributes on purpose: min-api attribute
    // construction invokes the custom setter with the default value, and
    // members are initialized in declaration order — everything a setter
    // touches must already be alive.
    std::unique_ptr<tap::ambi::dsp::format_converter> m_converter;
    long                                              m_channel_count{4};

  public:
    explicit ambitap_format(const atoms& args = {}) {
        int order = 1;
        if (!args.empty()) {
            order = std::clamp(static_cast<int>(args[0]), 0, 3);
        }
        m_converter     = std::make_unique<tap::ambi::dsp::format_converter>(order);
        m_channel_count = static_cast<long>(m_converter->channels());
    }

    attribute<symbol> direction{this, "direction", "ambix_to_fuma",
                                description{"Conversion direction: \"ambix_to_fuma\" or \"fuma_to_ambix\"."},
                                setter{MIN_FUNCTION{
                                    if (m_converter) {
                                        const std::string name = args[0];
                                        using dir              = tap::ambi::dsp::format_direction;
                                        m_converter->set_direction(name == "fuma_to_ambix" ? dir::fuma_to_ambix
                                                                                           : dir::ambix_to_fuma);
                                    }
                                    return args;
                                }}};

    message<> maxclass_setup{this, "maxclass_setup",
                             MIN_FUNCTION{
                                 c74::max::t_class* c = args[0];
                                 c74::max::class_addmethod(c, reinterpret_cast<c74::max::method>(mc_outputs),
                                                           "multichanneloutputs", c74::max::A_CANT, 0);
                                 return {};
                             }};

    void operator()(audio_bundle input, audio_bundle output) {
        const auto frames = input.frame_count();
        const auto in_ch  = input.channel_count();
        const auto out_ch = output.channel_count();

        for (long oc = 0; oc < out_ch; ++oc) {
            double* o = output.samples(oc);
            if (oc < m_channel_count) {
                const long   ic = static_cast<long>(m_converter->input_index(static_cast<size_t>(oc)));
                const double g  = m_converter->input_gain(static_cast<size_t>(oc));
                if (ic < in_ch) {
                    const double* s = input.samples(ic);
                    for (auto i = 0; i < frames; ++i) {
                        o[i] = s[i] * g;
                    }
                }
                else {
                    for (auto i = 0; i < frames; ++i) {
                        o[i] = 0.0;
                    }
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
    static long mc_outputs(minwrap<ambitap_format>* self, long) { return self->m_min_object.m_channel_count; }
};

MIN_EXTERNAL(ambitap_format);
