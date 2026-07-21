/// @file
/// ambitap.encode~ — encode a mono source into higher-order ambisonics
/// (AmbiX: ACN ordering, SN3D normalization).
///
/// The ambisonics order is a creation argument (default 1), and the output is a
/// single **multichannel** signal carrying (order+1)^2 channels — one patch
/// cord for the whole HOA bus. DSP lives in tap::ambi::dsp::encoder; this object
/// is min-api glue plus the Max `multichanneloutputs` negotiation (which min-api
/// does not wrap, so we register it ourselves at class-setup time).
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include <algorithm>
#include <memory>

#include "ambitap/dsp/encoder.h"
#include "c74_min.h"

using namespace c74::min;

class ambitap_encode : public object<ambitap_encode>, public vector_operator<> {
  public:
    MIN_DESCRIPTION{"Encode a mono source to higher-order ambisonics (AmbiX: ACN/SN3D). "
                    "Order is a creation argument; the output is one multichannel signal "
                    "of (order+1)^2 channels."};
    MIN_TAGS{"audio, ambisonics, spatial, mc"};
    MIN_AUTHOR{"Timothy Place"};
    MIN_RELATED{"ambitap.decode~, ambitap.rotate~, ambitap.binaural~"};

    inlet<>  m_in{this, "(signal) mono source"};
    outlet<> m_out{this, "(multichannelsignal) HOA bus, (order+1)^2 channels (ACN/SN3D)", "signal"};

  private:
    // State lives ABOVE the attributes on purpose: min-api attribute
    // construction invokes the custom setter with the default value, and
    // members are initialized in declaration order — everything a setter
    // touches must already be alive.
    std::unique_ptr<tap::ambi::dsp::encoder> m_encoder;
    long                                   m_channel_count{4};

  public:
    /// First creation argument is the ambisonics order (default 1).
    explicit ambitap_encode(const atoms& args = {}) {
        int order = 1;
        if (!args.empty()) {
            order = std::clamp(static_cast<int>(args[0]), 0, tap::ambi::k_max_order);
        }
        m_encoder       = std::make_unique<tap::ambi::dsp::encoder>(order);
        m_channel_count = static_cast<long>(m_encoder->channels());
    }

    attribute<number> azimuth{this, "azimuth", 0.0,
                              description{"Source azimuth in radians. 0 = front, pi/2 = left, pi = behind."},
                              setter{MIN_FUNCTION{
                                  const double value = args[0];
                                  if (m_encoder) {
                                      m_encoder->set_azimuth(static_cast<float>(value));
                                  }
                                  return {value};
                              }}};

    attribute<number> elevation{this, "elevation", 0.0,
                                description{"Source elevation in radians. 0 = horizon, pi/2 = zenith."},
                                setter{MIN_FUNCTION{
                                    const double value = args[0];
                                    if (m_encoder) {
                                        m_encoder->set_elevation(static_cast<float>(value));
                                    }
                                    return {value};
                                }}};

    attribute<number> gain{this, "gain", 1.0, description{"Linear output gain."},
                           setter{MIN_FUNCTION{
                               const double value = args[0];
                               if (m_encoder) {
                                   m_encoder->set_gain(static_cast<float>(value));
                               }
                               return {value};
                           }}};

    /// Registered at class-setup so the single signal outlet reports
    /// (order+1)^2 channels — i.e. behaves as a multichannel output. min-api
    /// does not wrap Max's `multichanneloutputs`, so we add it via raw max-api.
    message<> maxclass_setup{this, "maxclass_setup",
                             MIN_FUNCTION{
                                 c74::max::t_class* c = args[0];
                                 c74::max::class_addmethod(c, reinterpret_cast<c74::max::method>(mc_outputs),
                                                           "multichanneloutputs", c74::max::A_CANT, 0);
                                 return {};
                             }};

    /// out[ch] = in * encoder_coefficient[ch] (gain folded in). Channel count is
    /// whatever Max allocated from multichanneloutputs (== (order+1)^2).
    void operator()(audio_bundle input, audio_bundle output) {
        const auto    frames = input.frame_count();
        const auto    nch    = output.channel_count();
        const double* in     = input.samples(0);

        for (auto ch = 0; ch < nch; ++ch) {
            double*      out = output.samples(ch);
            const double g   = (ch < m_channel_count) ? m_encoder->channel_gain(ch) : 0.0;
            for (auto i = 0; i < frames; ++i) {
                out[i] = in[i] * g;
            }
        }
    }

  private:
    /// Max calls this (per outlet) to learn the channel count. Signature is
    /// long(t_object*, long); the t_object is the min wrapper instance.
    static long mc_outputs(minwrap<ambitap_encode>* self, long /* outlet_index */) {
        return self->m_min_object.m_channel_count;
    }
};

MIN_EXTERNAL(ambitap_encode);
