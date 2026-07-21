/// @file
/// ambitap.directional~ — apply a per-direction gain to a higher-order
/// ambisonics bus (extract at a direction, scale, re-encode, add back).
/// Order is a creation argument; multichannel in and out.
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include <algorithm>
#include <memory>
#include <vector>

#include "ambitap/dsp/directional_loudness.h"
#include "c74_min.h"

using namespace c74::min;

class ambitap_directional : public object<ambitap_directional>, public mc_operator<> {
  public:
    MIN_DESCRIPTION{"Apply a per-direction gain to a higher-order ambisonics bus "
                    "(gain = 1 bypass, > 1 boost, < 1 attenuate). Order is a creation arg."};
    MIN_TAGS{"audio, ambisonics, spatial, gain, mc"};
    MIN_AUTHOR{"Timothy Place"};
    MIN_RELATED{"ambitap.vmic~, ambitap.mirror~, ambitap.encode~"};

    inlet<>  m_in{this, "(multichannelsignal) HOA bus"};
    outlet<> m_out{this, "(multichannelsignal) shaped HOA bus", "signal"};

  private:
    // State lives ABOVE the attributes on purpose: min-api attribute
    // construction invokes the custom setter with the default value, and
    // members are initialized in declaration order — everything a setter
    // touches must already be alive.
    std::unique_ptr<tap::ambi::dsp::directional_loudness> m_loudness;
    long                                                m_channel_count{4};
    std::vector<float>                                  m_in_frame;
    std::vector<float>                                  m_out_frame;

  public:
    explicit ambitap_directional(const atoms& args = {}) {
        int order = 1;
        if (!args.empty()) {
            order = std::clamp(static_cast<int>(args[0]), 1, tap::ambi::k_max_order);
        }
        m_loudness      = std::make_unique<tap::ambi::dsp::directional_loudness>(order);
        m_channel_count = static_cast<long>(m_loudness->channels());
        m_in_frame.assign(static_cast<size_t>(m_channel_count), 0.0f);
        m_out_frame.assign(static_cast<size_t>(m_channel_count), 0.0f);
    }

    attribute<number> azimuth{this, "azimuth", 0.0,
                              description{"Boost-direction azimuth in radians. 0 = front, pi/2 = left."},
                              setter{MIN_FUNCTION{
                                  const double v = args[0];
                                  if (m_loudness) {
                                      m_loudness->set_azimuth(static_cast<float>(v));
                                  }
                                  return {v};
                              }}};

    attribute<number> elevation{this, "elevation", 0.0,
                                description{"Boost-direction elevation in radians. 0 = horizon, pi/2 = zenith."},
                                setter{MIN_FUNCTION{
                                    const double v = args[0];
                                    if (m_loudness) {
                                        m_loudness->set_elevation(static_cast<float>(v));
                                    }
                                    return {v};
                                }}};

    attribute<number> gain{this, "gain", 1.0,
                           description{"Linear gain at the look direction. 1 = bypass, 0 = attenuate, >1 = boost."},
                           setter{MIN_FUNCTION{
                               const double v = args[0];
                               if (m_loudness) {
                                   m_loudness->set_gain(static_cast<float>(v));
                               }
                               return {v};
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
        const long n      = m_channel_count;

        for (auto i = 0; i < frames; ++i) {
            for (long c = 0; c < n; ++c) {
                m_in_frame[c] = (c < in_ch) ? static_cast<float>(input.samples(c)[i]) : 0.0f;
            }
            m_loudness->process_frame(m_in_frame.data(), m_out_frame.data());
            for (long c = 0; c < out_ch; ++c) {
                output.samples(c)[i] = (c < n) ? static_cast<double>(m_out_frame[c]) : 0.0;
            }
        }
    }

  private:
    static long mc_outputs(minwrap<ambitap_directional>* self, long) { return self->m_min_object.m_channel_count; }
};

MIN_EXTERNAL(ambitap_directional);
