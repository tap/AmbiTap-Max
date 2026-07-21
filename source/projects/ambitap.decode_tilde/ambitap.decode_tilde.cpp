/// @file
/// ambitap.decode~ — decode a higher-order ambisonics bus to a loudspeaker
/// layout (AmbiX: ACN/SN3D).
///
/// Creation args: <order> <layout>. Order defaults to 1; layout defaults to
/// "stereo" and is one of: stereo, quad, surround_5_1, hexagon, surround_7_1,
/// cube, octagon, surround_7_1_4. The layout fixes the output channel count
/// (number of speakers) at construction; the output is one multichannel signal
/// of that many speaker feeds (→ mc.dac~).
///
/// Runtime attributes `decoder_type` (mode_match / allrad / epad) and `max_re`
/// only rebuild the decode matrix — they do not change the channel count. The
/// SVD / T-design+VBAP construction runs off the audio thread
/// (tap::ambi::dsp::decoder's async_rebuilder); the audio path reads wait-free.
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "ambitap/dsp/decoder.h"
#include "ambitap/math/geometry/layouts.h"
#include "c74_min.h"

using namespace c74::min;

class ambitap_decode : public object<ambitap_decode>, public mc_operator<> {
  public:
    MIN_DESCRIPTION{"Decode a higher-order ambisonics bus to a loudspeaker layout. "
                    "Creation args: <order> <layout>. The layout fixes the output channel "
                    "count; the output is one multichannel signal of speaker feeds."};
    MIN_TAGS{"audio, ambisonics, spatial, mc"};
    MIN_AUTHOR{"Timothy Place"};
    MIN_RELATED{"ambitap.encode~, ambitap.rotate~, ambitap.binaural~"};

    inlet<>  m_in{this, "(multichannelsignal) HOA bus"};
    outlet<> m_out{this, "(multichannelsignal) loudspeaker feeds", "signal"};

  private:
    // State lives ABOVE the attributes on purpose: min-api attribute
    // construction invokes the custom setter with the default value, and
    // members are initialized in declaration order — everything a setter
    // touches must already be alive.
    std::unique_ptr<tap::ambi::dsp::decoder> m_decoder;
    long                                   m_in_channels{4};
    long                                   m_speaker_count{2};
    std::vector<float>                     m_in_frame;
    std::vector<float>                     m_out_frame;

  public:
    /// Creation args: <order> <layout-name>.
    explicit ambitap_decode(const atoms& args = {}) {
        int order = 1;
        if (args.size() >= 1) {
            order = std::clamp(static_cast<int>(args[0]), 1, tap::ambi::k_max_order);
        }

        std::string layout_name = "stereo";
        if (args.size() >= 2) {
            layout_name = static_cast<std::string>(args[1]);
        }

        auto speakers = layout_from_name(layout_name);
        if (speakers.empty()) {
            speakers = tap::ambi::layouts::stereo(); // unknown name -> stereo
        }

        m_decoder       = std::make_unique<tap::ambi::dsp::decoder>(order);
        m_in_channels   = static_cast<long>(m_decoder->input_channels());
        m_speaker_count = static_cast<long>(speakers.size());
        m_decoder->set_speakers(speakers); // triggers the first matrix build

        m_in_frame.assign(static_cast<size_t>(m_in_channels), 0.0f);
        m_out_frame.assign(static_cast<size_t>(m_speaker_count), 0.0f);
    }

    attribute<symbol> decoder_type{
        this, "decoder_type", "mode_match",
        description{"Decoder algorithm: mode_match (pseudoinverse), allrad (T-design + VBAP), "
                    "or epad (energy-preserving)."},
        setter{MIN_FUNCTION{
            if (m_decoder) {
                const std::string name = args[0];
                m_decoder->set_algorithm(algorithm_from_name(name));
            }
            return args;
        }}};

    attribute<bool> max_re{this, "max_re", false, description{"Apply per-order max-rE weighting to reduce side lobes."},
                           setter{MIN_FUNCTION{
                               const bool on = args[0];
                               if (m_decoder) {
                                   m_decoder->set_max_re(on);
                               }
                               return {on};
                           }}};

    /// Register Max's multichanneloutputs so the output reports the speaker count.
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

        for (auto i = 0; i < frames; ++i) {
            for (long c = 0; c < m_in_channels; ++c) {
                m_in_frame[c] = (c < in_ch) ? static_cast<float>(input.samples(c)[i]) : 0.0f;
            }

            m_decoder->process_frame(m_in_frame.data(), m_out_frame.data(), static_cast<size_t>(m_speaker_count));

            for (long c = 0; c < out_ch; ++c) {
                output.samples(c)[i] = (c < m_speaker_count) ? static_cast<double>(m_out_frame[c]) : 0.0;
            }
        }
    }

  private:
    static std::vector<tap::ambi::spherical_coord> layout_from_name(const std::string& name) {
        using namespace tap::ambi::layouts;
        if (name == "stereo") {
            return stereo();
        }
        if (name == "quad") {
            return quad();
        }
        if (name == "surround_5_1" || name == "5.1") {
            return surround_5_1();
        }
        if (name == "surround_7_1" || name == "7.1") {
            return surround_7_1();
        }
        if (name == "surround_7_1_4" || name == "7.1.4") {
            return surround_7_1_4();
        }
        if (name == "cube") {
            return cube();
        }
        if (name == "hexagon") {
            return hexagon();
        }
        if (name == "octagon") {
            return octagon();
        }
        return {};
    }

    static tap::ambi::dsp::decoder_algorithm algorithm_from_name(const std::string& name) {
        if (name == "allrad") {
            return tap::ambi::dsp::decoder_algorithm::allrad;
        }
        if (name == "epad") {
            return tap::ambi::dsp::decoder_algorithm::epad;
        }
        return tap::ambi::dsp::decoder_algorithm::mode_match;
    }

    static long mc_outputs(minwrap<ambitap_decode>* self, long /* outlet_index */) {
        return self->m_min_object.m_speaker_count;
    }
};

MIN_EXTERNAL(ambitap_decode);
