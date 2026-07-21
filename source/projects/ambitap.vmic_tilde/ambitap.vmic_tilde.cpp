/// @file
/// ambitap.vmic~ — virtual microphone: extract a mono directional signal from a
/// higher-order ambisonics bus (cardioid at order 1, narrower beams higher).
/// Order is a creation argument; multichannel in, mono out.
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include <algorithm>
#include <memory>
#include <vector>

#include "ambitap/dsp/virtual_mic.h"
#include "c74_min.h"

using namespace c74::min;

class ambitap_vmic : public object<ambitap_vmic>, public mc_operator<> {
  public:
    MIN_DESCRIPTION{"Extract a mono directional signal from a higher-order ambisonics bus. "
                    "Cardioid at order 1, narrower beams at higher orders. Order is a "
                    "creation argument."};
    MIN_TAGS{"audio, ambisonics, spatial, analysis, mc"};
    MIN_AUTHOR{"Timothy Place"};
    MIN_RELATED{"ambitap.encode~, ambitap.directional~, ambitap.energyvec~"};

    inlet<>  m_in{this, "(multichannelsignal) HOA bus"};
    outlet<> m_out{this, "(signal) extracted mono", "signal"};

  private:
    // State lives ABOVE the attributes on purpose: min-api attribute
    // construction invokes the custom setter with the default value, and
    // members are initialized in declaration order — everything a setter
    // touches must already be alive.
    std::unique_ptr<tap::ambi::dsp::virtual_mic> m_mic;
    long                                       m_channels{4};
    std::vector<float>                         m_in_frame;

  public:
    explicit ambitap_vmic(const atoms& args = {}) {
        int order = 1;
        if (!args.empty()) {
            order = std::clamp(static_cast<int>(args[0]), 1, tap::ambi::k_max_order);
        }
        m_mic      = std::make_unique<tap::ambi::dsp::virtual_mic>(order);
        m_channels = static_cast<long>(m_mic->channels());
        m_in_frame.assign(static_cast<size_t>(m_channels), 0.0f);
    }

    attribute<number> azimuth{this, "azimuth", 0.0,
                              description{"Look-direction azimuth in radians. 0 = front, pi/2 = left."},
                              setter{MIN_FUNCTION{
                                  const double v = args[0];
                                  if (m_mic) {
                                      m_mic->set_azimuth(static_cast<float>(v));
                                  }
                                  return {v};
                              }}};

    attribute<number> elevation{this, "elevation", 0.0,
                                description{"Look-direction elevation in radians. 0 = horizon, pi/2 = zenith."},
                                setter{MIN_FUNCTION{
                                    const double v = args[0];
                                    if (m_mic) {
                                        m_mic->set_elevation(static_cast<float>(v));
                                    }
                                    return {v};
                                }}};

    attribute<bool> max_re{this, "max_re", false, description{"Apply per-order max-rE weighting to smooth sidelobes."},
                           setter{MIN_FUNCTION{
                               const bool on = args[0];
                               if (m_mic) {
                                   m_mic->set_max_re(on);
                               }
                               return {on};
                           }}};

    void operator()(audio_bundle input, audio_bundle output) {
        const auto frames = input.frame_count();
        const auto in_ch  = input.channel_count();
        double*    o      = output.samples(0);

        for (auto i = 0; i < frames; ++i) {
            for (long c = 0; c < m_channels; ++c) {
                m_in_frame[c] = (c < in_ch) ? static_cast<float>(input.samples(c)[i]) : 0.0f;
            }
            o[i] = m_mic->process_frame(m_in_frame.data());
        }
    }
};

MIN_EXTERNAL(ambitap_vmic);
