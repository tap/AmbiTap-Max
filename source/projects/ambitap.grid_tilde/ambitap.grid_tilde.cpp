/// @file
/// ambitap.grid~ — soundfield energy heatmap analysis for a higher-order
/// ambisonics bus (analysis::soundfield_grid). The HOA bus passes through
/// unchanged (insert it inline like a meter); `bang` emits the current
/// smoothed heatmap from the right outlet as
///
///     grid <rows> <cols> <peak_db> <v0> <v1> ... (row-major, values 0..1,
///     row 0 = zenith, column 0 = azimuth -pi)
///
/// — the message the AmbiTap UI layer's v8ui heatmap widget consumes
/// directly (AmbiTap ui/UI.md; drive the bang from qmetro at display rate).
/// Order is a creation argument; @azimuth_steps sets the grid resolution
/// (columns; rows are half that), @dynamic_range the dB window below the
/// peak that maps to 0..1.
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include <algorithm>
#include <memory>
#include <vector>

#include "ambitap/analysis/soundfield_grid.h"
#include "c74_min.h"

using namespace c74::min;

class ambitap_grid : public object<ambitap_grid>, public mc_operator<> {
  public:
    MIN_DESCRIPTION{"Soundfield energy heatmap analysis for a HOA bus (equirectangular grid). "
                    "The bus passes through; bang outputs the smoothed grid as a list for the "
                    "AmbiTap v8ui heatmap widget. Order is a creation argument."};
    MIN_TAGS{"audio, ambisonics, spatial, analysis, mc"};
    MIN_AUTHOR{"Timothy Place"};
    MIN_RELATED{"ambitap.energyvec~, ambitap.vmic~"};

    inlet<>  m_in{this, "(multichannelsignal) HOA bus; bang to output the grid"};
    outlet<> m_thru{this, "(multichannelsignal) HOA bus passthrough", "signal"};
    outlet<> m_grid_out{this, "(list) grid <rows> <cols> <peak_db> <values...> on bang"};

  private:
    // State lives ABOVE the attributes on purpose: min-api attribute
    // construction invokes the custom setter with the default value, and
    // members are initialized in declaration order — everything a setter
    // touches must already be alive.
    std::unique_ptr<tap::ambi::analysis::soundfield_grid> m_grid;
    long                                                m_channel_count{4};
    std::vector<float>                                  m_planar; // sized in dspsetup, reused (RT-safe)
    std::vector<const float*>                           m_ptrs;
    long                                                m_max_frames{0};

  public:
    /// First creation argument is the ambisonics order (default 1).
    explicit ambitap_grid(const atoms& args = {}) {
        int order = 1;
        if (!args.empty()) {
            order = std::clamp(static_cast<int>(args[0]), 0, tap::ambi::k_max_order);
        }
        m_grid          = std::make_unique<tap::ambi::analysis::soundfield_grid>(order);
        m_channel_count = static_cast<long>(m_grid->channels());
        m_ptrs.assign(static_cast<size_t>(m_channel_count), nullptr);
    }

    attribute<int> azimuth_steps{
        this, "azimuth_steps", 32,
        description{"Grid azimuth resolution (columns; rows are half). Rounded to even, 4-128. "
                    "Rebuilds the SH table on the control thread."},
        setter{MIN_FUNCTION{
            int steps = std::clamp(static_cast<int>(args[0]), 4, 128) & ~1;
            if (m_grid) {
                m_grid->set_azimuth_steps(steps);
            }
            return {steps};
        }}};

    attribute<number> smoothing_time{this, "smoothing_time", 200.0,
                                     description{"Per-direction energy smoothing time constant, milliseconds."},
                                     setter{MIN_FUNCTION{
                                         const double v = args[0];
                                         if (m_grid) {
                                             m_grid->set_smoothing_time_ms(static_cast<float>(v));
                                         }
                                         return {v};
                                     }}};

    attribute<number> dynamic_range{this, "dynamic_range", 40.0,
                                    description{"Display dynamic range in dB below the peak mapped to 0..1."}};

    /// Snapshot the smoothed grid (any-thread safe) and send it as one list.
    message<> bang{this, "bang", "Output the current grid snapshot.",
                   MIN_FUNCTION{
                       if (!m_grid) {
                           return {};
                       }
                       const auto image = m_grid->snapshot(static_cast<float>(static_cast<double>(dynamic_range)));
                       if (image.data.empty()) {
                           return {};
                       }
                       atoms out;
                       out.reserve(4 + image.data.size());
                       out.push_back("grid");
                       out.push_back(image.rows);
                       out.push_back(image.cols);
                       out.push_back(image.peak_db);
                       for (const float v : image.data) {
                           out.push_back(v);
                       }
                       m_grid_out.send(out);
                       return {};
                   }};

    /// Smoothing tracks the host sample rate; planar scratch tracks the vector
    /// size so the audio path never allocates.
    message<> dspsetup{this, "dspsetup",
                       MIN_FUNCTION{
                           const double sr          = args[0];
                           const int    vector_size = args[1];
                           m_grid->prepare(static_cast<float>(sr));
                           m_max_frames = vector_size;
                           m_planar.assign(static_cast<size_t>(m_channel_count) * static_cast<size_t>(vector_size),
                                           0.0f);
                           return {};
                       }};

    /// Register Max's multichanneloutputs so the passthrough reports (order+1)^2
    /// channels. (min-api wraps MC inlets, but not MC outputs.)
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

        // Passthrough (double precision, zero-padded to the bus width).
        for (long c = 0; c < out_ch; ++c) {
            double* out = output.samples(c);
            if (c < in_ch) {
                const double* in = input.samples(c);
                std::copy_n(in, frames, out);
            }
            else {
                std::fill_n(out, frames, 0.0);
            }
        }

        // Gather planar float channels for the analyzer (allocation-free).
        if (m_planar.empty() || frames > m_max_frames) {
            return; // dsp not set up yet
        }
        for (long c = 0; c < n; ++c) {
            float* dst = m_planar.data() + static_cast<size_t>(c) * static_cast<size_t>(m_max_frames);
            if (c < in_ch) {
                const double* in = input.samples(c);
                for (auto i = 0; i < frames; ++i) {
                    dst[i] = static_cast<float>(in[i]);
                }
            }
            else {
                std::fill_n(dst, frames, 0.0f);
            }
            m_ptrs[static_cast<size_t>(c)] = dst;
        }
        m_grid->process(m_ptrs.data(), static_cast<size_t>(frames));
    }

  private:
    static long mc_outputs(minwrap<ambitap_grid>* self, long /* outlet_index */) {
        return self->m_min_object.m_channel_count;
    }
};

MIN_EXTERNAL(ambitap_grid);
