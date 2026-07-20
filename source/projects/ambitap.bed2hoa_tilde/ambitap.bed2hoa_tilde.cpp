/// @file
/// ambitap.bed2hoa~ — encode a channel-based surround bed (5.1, 7.1, 7.1.4, …)
/// into higher-order ambisonics (AmbiX: ACN ordering, SN3D normalization) by
/// encoding each speaker feed at its canonical direction.
///
/// Creation args: <order> <layout>. Order defaults to 1; layout defaults to
/// "surround_5_1" and is one of: stereo, quad, surround_5_1, hexagon,
/// surround_7_1, cube, octagon, surround_7_1_4 (the same set as
/// ambitap.decode~, so bed2hoa~ → decode~ round-trips a bed through the HOA
/// domain). Input is one multichannel signal carrying the layout's speaker
/// feeds in layout order; output is one multichannel signal of (order+1)^2
/// channels. The encoding matrix is static (canonical directions), so the
/// audio path is a plain matrix multiply — wait-free, no allocation.
///
/// LFE is not a directional signal and is not part of the layouts: feed the
/// speaker channels only (5 for 5.1, 7 for 7.1, 11 for 7.1.4) and route any
/// LFE around the ambisonic bus.
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include <algorithm>
#include <string>
#include <vector>

#include "ambitap/math/core/spherical_harmonics.h"
#include "ambitap/math/geometry/layouts.h"
#include "c74_min.h"

using namespace c74::min;

class ambitap_bed2hoa : public object<ambitap_bed2hoa>, public mc_operator<> {
  public:
    MIN_DESCRIPTION{"Encode a channel-based surround bed to higher-order ambisonics "
                    "(AmbiX: ACN/SN3D) by encoding each speaker feed at its canonical "
                    "direction. Creation args: <order> <layout>."};
    MIN_TAGS{"audio, ambisonics, spatial, surround, mc"};
    MIN_AUTHOR{"Timothy Place"};
    MIN_RELATED{"ambitap.encode~, ambitap.decode~, ambitap.rotate~, ambitap.binaural~"};

    inlet<>  m_in{this, "(multichannelsignal) speaker feeds in layout order (no LFE)"};
    outlet<> m_out{this, "(multichannelsignal) HOA bus, (order+1)^2 channels (ACN/SN3D)", "signal"};

    /// Creation args: <order> <layout-name>.
    explicit ambitap_bed2hoa(const atoms& args = {}) {
        int order = 1;
        if (!args.empty()) {
            order = std::clamp(static_cast<int>(args[0]), 0, ambitap::k_max_order);
        }

        std::string layout_name = "surround_5_1";
        if (args.size() > 1) {
            layout_name = static_cast<std::string>(args[1]);
        }

        auto speakers = layout_from_name(layout_name);
        if (speakers.empty()) {
            speakers = ambitap::layouts::surround_5_1(); // unknown name -> 5.1
        }

        m_channel_count = static_cast<long>(ambitap::channel_count(order));
        m_speaker_count = static_cast<long>(speakers.size());

        // Static encoding matrix: gains[s][ch] = SH_ch at speaker direction s.
        m_gains.assign(static_cast<size_t>(m_speaker_count),
                       std::vector<double>(static_cast<size_t>(m_channel_count), 0.0));
        float sh[ambitap::k_max_channel_count];
        for (size_t s = 0; s < static_cast<size_t>(m_speaker_count); ++s) {
            ambitap::evaluate_sh(order, speakers[s].azimuth, speakers[s].elevation, sh);
            for (size_t ch = 0; ch < static_cast<size_t>(m_channel_count); ++ch) {
                m_gains[s][ch] = static_cast<double>(sh[ch]);
            }
        }
    }

    attribute<number> gain{this, "gain", 1.0, description{"Linear output gain applied to the encoded bus."}};

    /// Registered at class-setup so the single signal outlet reports
    /// (order+1)^2 channels — the same multichanneloutputs pattern as
    /// ambitap.encode~.
    message<> maxclass_setup{this, "maxclass_setup",
                             MIN_FUNCTION{
                                 c74::max::t_class* c = args[0];
                                 c74::max::class_addmethod(c, reinterpret_cast<c74::max::method>(mc_outputs),
                                                           "multichanneloutputs", c74::max::A_CANT, 0);
                                 return {};
                             }};

    /// out[ch] = gain * sum over speakers of in[s] * G[s][ch]. Input channels
    /// beyond the layout's speaker count are ignored; missing ones are silent.
    void operator()(audio_bundle input, audio_bundle output) {
        const auto   frames = input.frame_count();
        const auto   in_ch  = std::min(input.channel_count(), m_speaker_count);
        const auto   out_ch = output.channel_count();
        const double g      = gain;

        for (auto ch = 0; ch < out_ch; ++ch) {
            double* out = output.samples(ch);
            for (auto i = 0; i < frames; ++i) {
                out[i] = 0.0;
            }
            if (ch >= m_channel_count) {
                continue;
            }
            for (auto s = 0; s < in_ch; ++s) {
                const double  gs = m_gains[static_cast<size_t>(s)][static_cast<size_t>(ch)] * g;
                const double* in = input.samples(s);
                for (auto i = 0; i < frames; ++i) {
                    out[i] += in[i] * gs;
                }
            }
        }
    }

  private:
    long                             m_channel_count{4};
    long                             m_speaker_count{5};
    std::vector<std::vector<double>> m_gains; // [speaker][hoa channel]

    static std::vector<ambitap::spherical_coord> layout_from_name(const std::string& name) {
        using namespace ambitap::layouts;
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

    /// Max calls this (per outlet) to learn the channel count.
    static long mc_outputs(minwrap<ambitap_bed2hoa>* self, long /* outlet_index */) {
        return self->m_min_object.m_channel_count;
    }
};

MIN_EXTERNAL(ambitap_bed2hoa);
