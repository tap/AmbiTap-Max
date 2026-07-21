/// @file
/// ambitap.room~ — shoebox room simulation on a higher-order ambisonics bus:
/// mono source in, (order+1)^2 SH channels out (AmbiX: ACN/SN3D), as direct
/// sound + image-source early reflections + a 16-line SH-domain FDN late
/// tail. Order is a creation argument (default 1, max 3).
///
/// DSP lives in tap::ambi::dsp::room — the real-time realization of the
/// architecture verified against the R1-R10 gates in the AmbiTap library's
/// docs/PERCEPTUAL-VERIFICATION.md. Geometry / RT60 changes are rebuilt on
/// the library's worker thread and crossfaded in; the audio path is
/// wait-free. Convolvers are (re)allocated for the host's signal vector size
/// and sample rate in `dspsetup` (power-of-two vectors only; the injection
/// convolution is partitioned FFT); the object is silent until then.
///
/// Note the fixed latency on every path (direct sound included):
/// room.latency_samples() = max(3989 - round(0.030 * samplerate), 0) samples
/// (~53 ms at 48 kHz) — the causality cost of the FDN's injection alignment.
/// Compensate when mixing with undelayed signals.
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include <algorithm>
#include <array>
#include <memory>
#include <vector>

#include "ambitap/dsp/room.h"
#include "c74_min.h"

using namespace c74::min;

class ambitap_room : public object<ambitap_room>, public vector_operator<> {
  public:
    MIN_DESCRIPTION{"Shoebox room simulation for a mono source: image-source early "
                    "reflections + FDN late reverb, output as one multichannel HOA bus "
                    "of (order+1)^2 channels (AmbiX: ACN/SN3D). Order is a creation "
                    "argument (max 3)."};
    MIN_TAGS{"audio, ambisonics, spatial, reverb, room, mc"};
    MIN_AUTHOR{"Timothy Place"};
    MIN_RELATED{"ambitap.encode~, ambitap.binaural~, ambitap.decode~, ambitap.distance~"};

    inlet<>  m_in{this, "(signal) mono source"};
    outlet<> m_out{this, "(multichannelsignal) HOA bus, (order+1)^2 channels (ACN/SN3D)", "signal"};

  private:
    // State lives ABOVE the attributes on purpose: min-api attribute
    // construction invokes the custom setter with the default value, and
    // members are initialized in declaration order — everything a setter
    // touches must already be alive.

    /// One-pole coefficient of the per-sample output-gain slew (~5 ms at 48 kHz).
    static constexpr float k_gain_slew = 1.0f / 256.0f;

    std::unique_ptr<tap::ambi::dsp::room> m_room;
    long                                m_channel_count{4};
    long                                m_block_size{0};
    float                               m_gain_smooth{1.0f};
    std::vector<float>                  m_in_buf;
    std::vector<std::vector<float>>     m_out_bufs; // [channel][block]
    std::vector<float*>                 m_out_ptrs; // into m_out_bufs

  public:
    /// First creation argument is the ambisonics order (default 1, max 3 —
    /// the 16 FDN lines feed at most the 16 channels of order 3).
    explicit ambitap_room(const atoms& args = {}) {
        int order = 1;
        if (!args.empty()) {
            order = std::clamp(static_cast<int>(args[0]), 0, tap::ambi::dsp::room::k_max_room_order);
        }
        m_room          = std::make_unique<tap::ambi::dsp::room>(order);
        m_channel_count = static_cast<long>(m_room->channels());
    }

    // Attribute defaults mirror dsp::room's defaults (the library's verified
    // seed-11 configuration); the setters run before m_room exists during
    // construction, so the two default sets must match.

    attribute<number> dim_x{this, "dim_x", 7.10,
                            description{"Room depth in meters (x, front axis). Clamped to 1..50 m."},
                            setter{MIN_FUNCTION{
                                const double v = args[0];
                                push_dimensions(v, dim_y, dim_z);
                                return {v};
                            }}};

    attribute<number> dim_y{this, "dim_y", 5.30,
                            description{"Room width in meters (y, left axis). Clamped to 1..50 m."},
                            setter{MIN_FUNCTION{
                                const double v = args[0];
                                push_dimensions(dim_x, v, dim_z);
                                return {v};
                            }}};

    attribute<number> dim_z{this, "dim_z", 3.10, description{"Room height in meters (z, up axis). Clamped to 1..50 m."},
                            setter{MIN_FUNCTION{
                                const double v = args[0];
                                push_dimensions(dim_x, dim_y, v);
                                return {v};
                            }}};

    attribute<number> source_x{this, "source_x", 3.674,
                               description{"Source position x in meters from the room's origin corner."},
                               setter{MIN_FUNCTION{
                                   const double v = args[0];
                                   push_source(v, source_y, source_z);
                                   return {v};
                               }}};

    attribute<number> source_y{this, "source_y", 1.137,
                               description{"Source position y in meters from the room's origin corner."},
                               setter{MIN_FUNCTION{
                                   const double v = args[0];
                                   push_source(source_x, v, source_z);
                                   return {v};
                               }}};

    attribute<number> source_z{this, "source_z", 1.977,
                               description{"Source position z in meters from the room's origin corner."},
                               setter{MIN_FUNCTION{
                                   const double v = args[0];
                                   push_source(source_x, source_y, v);
                                   return {v};
                               }}};

    attribute<number> listener_x{this, "listener_x", 1.746,
                                 description{"Listener position x in meters from the room's origin corner."},
                                 setter{MIN_FUNCTION{
                                     const double v = args[0];
                                     push_listener(v, listener_y, listener_z);
                                     return {v};
                                 }}};

    attribute<number> listener_y{this, "listener_y", 1.711,
                                 description{"Listener position y in meters from the room's origin corner."},
                                 setter{MIN_FUNCTION{
                                     const double v = args[0];
                                     push_listener(listener_x, v, listener_z);
                                     return {v};
                                 }}};

    attribute<number> listener_z{this, "listener_z", 0.668,
                                 description{"Listener position z in meters from the room's origin corner."},
                                 setter{MIN_FUNCTION{
                                     const double v = args[0];
                                     push_listener(listener_x, listener_y, v);
                                     return {v};
                                 }}};

    attribute<number> rt60{this, "rt60", 0.76,
                           description{"Broadband reverb time in seconds (0.1..10): scales every octave "
                                       "band so the 1 kHz band lands here, preserving the spectral tilt. "
                                       "Per-band control via the rt60band message."},
                           setter{MIN_FUNCTION{
                               const double v = args[0];
                               if (m_room) {
                                   m_room->set_rt60(static_cast<float>(v));
                               }
                               return {v};
                           }}};

    attribute<bool> direct{this, "direct", true,
                           description{"Enable the direct-sound path (off = reflections/reverb only; "
                                       "compose with ambitap.encode~ for an anechoic direct path)."},
                           setter{MIN_FUNCTION{
                               const bool on = args[0];
                               if (m_room) {
                                   m_room->set_direct_enabled(on);
                               }
                               return {on};
                           }}};

    attribute<bool> er{this, "er", true, description{"Enable the image-source early reflections (first 30 ms)."},
                       setter{MIN_FUNCTION{
                           const bool on = args[0];
                           if (m_room) {
                               m_room->set_early_enabled(on);
                           }
                           return {on};
                       }}};

    attribute<bool> tail{this, "tail", true, description{"Enable the FDN late-reverb tail (t >= 30 ms)."},
                         setter{MIN_FUNCTION{
                             const bool on = args[0];
                             if (m_room) {
                                 m_room->set_tail_enabled(on);
                             }
                             return {on};
                         }}};

    attribute<number> gain{this, "gain", 1.0, description{"Linear output gain (smoothed per sample)."},
                           setter{MIN_FUNCTION{
                               const double v = args[0];
                               return {std::max(v, 0.0)};
                           }}};

    attribute<symbol> absorption{this, "absorption", "fir",
                                 description{"Per-line loop absorption filter: \"fir\" (default) uses the verified "
                                             "255-tap linear-phase FIRs; \"iir\" swaps in one cheap first-order "
                                             "low-pass per line — markedly lower CPU, at the cost of approximate "
                                             "mid-band RT60 and a slightly different late texture (the tail stays "
                                             "level-calibrated either way). Rebuilds off-thread; may glitch briefly."},
                                 setter{MIN_FUNCTION{
                                     using kind     = tap::ambi::dsp::room::absorption_kind;
                                     const bool iir = (symbol(args[0]) == symbol("iir"));
                                     if (m_room) {
                                         m_room->set_absorption_kind(iir ? kind::iir : kind::fir);
                                     }
                                     return {iir ? symbol("iir") : symbol("fir")};
                                 }}};

    /// Per-band reverb time: rt60band <center_hz> <seconds>. Centers are the
    /// parameterized octave bands 250, 500, 1000, 2000, 4000 Hz.
    message<> rt60band{this, "rt60band", "Set one octave band's RT60: rt60band <center_hz> <seconds>.",
                       MIN_FUNCTION{
                           if (args.size() < 2 || !m_room) {
                               return {};
                           }
                           const double hz  = args[0];
                           const double sec = args[1];
                           for (size_t b = 0; b < tap::ambi::dsp::room::k_rt60_bands; ++b) {
                               if (std::abs(tap::ambi::dsp::room::k_rt60_centers_hz[b] - hz) < 1.0) {
                                   m_room->set_rt60_band(b, static_cast<float>(sec));
                                   return {};
                               }
                           }
                           cerr << "rt60band: no band at " << hz << " Hz (250/500/1000/2000/4000)" << endl;
                           return {};
                       }};

    /// Wall amplitude reflection coefficients (0..1), in x0 x1 y0 y1 z0 z1
    /// order: reflections <6 floats>. They shape the early-reflection levels
    /// and the tail's level calibration; RT60(f) is parameterized separately.
    message<> reflections{this, "reflections",
                          "Set the six wall reflection coefficients: reflections <x0 x1 y0 y1 z0 z1>.",
                          MIN_FUNCTION{
                              if (args.size() < 6 || !m_room) {
                                  return {};
                              }
                              std::array<float, tap::ambi::dsp::room::k_walls> c{};
                              for (size_t w = 0; w < c.size(); ++w) {
                                  c[w] = static_cast<float>(static_cast<double>(args[w]));
                              }
                              m_room->set_wall_reflections(c);
                              return {};
                          }};

    /// Allocate the room's audio state for the host's signal vector size and
    /// sample rate. Called by Max whenever the dsp chain is (re)compiled.
    message<> dspsetup{this, "dspsetup",
                       MIN_FUNCTION{
                           const double sample_rate = args[0];
                           const long   vector_size = args[1];
                           prepare_room(vector_size, sample_rate);
                           return {};
                       }};

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

    void operator()(audio_bundle input, audio_bundle output) {
        const auto frames = input.frame_count();
        const auto out_ch = output.channel_count();
        const long n      = m_channel_count;

        // The room needs the power-of-two vector size prepared in dspsetup;
        // emit silence if the current block doesn't match.
        if (!m_room->is_prepared() || frames != m_block_size) {
            for (long c = 0; c < out_ch; ++c) {
                double* out = output.samples(c);
                for (auto i = 0; i < frames; ++i) {
                    out[i] = 0.0;
                }
            }
            return;
        }

        const double* in = input.samples(0);
        for (auto i = 0; i < frames; ++i) {
            m_in_buf[static_cast<size_t>(i)] = static_cast<float>(in[i]);
        }

        m_room->process(m_in_buf.data(), m_out_ptrs.data(), static_cast<size_t>(frames));

        const float g_target = static_cast<float>(static_cast<double>(gain));
        for (long c = 0; c < out_ch; ++c) {
            double* out = output.samples(c);
            if (c < n) {
                const float* src = m_out_bufs[static_cast<size_t>(c)].data();
                float        g   = m_gain_smooth;
                for (auto i = 0; i < frames; ++i) {
                    g += (g_target - g) * k_gain_slew;
                    out[i] = static_cast<double>(src[i] * g);
                }
                if (c == n - 1) {
                    m_gain_smooth = g;
                }
            }
            else {
                for (auto i = 0; i < frames; ++i) {
                    out[i] = 0.0;
                }
            }
        }
    }

  private:
    void push_dimensions(double x, double y, double z) {
        if (m_room) {
            m_room->set_room_dimensions(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
        }
    }
    void push_source(double x, double y, double z) {
        if (m_room) {
            m_room->set_source_position(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
        }
    }
    void push_listener(double x, double y, double z) {
        if (m_room) {
            m_room->set_listener_position(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
        }
    }

    void prepare_room(long vector_size, double sample_rate) {
        const bool valid = (vector_size >= 4) && ((vector_size & (vector_size - 1)) == 0);
        if (!valid) {
            m_block_size = 0; // unsupported vector size -> stay silent
            return;
        }
        m_block_size = vector_size;
        m_room->prepare(static_cast<size_t>(vector_size), static_cast<float>(sample_rate));

        const auto v = static_cast<size_t>(vector_size);
        m_in_buf.assign(v, 0.0f);
        m_out_bufs.assign(static_cast<size_t>(m_channel_count), std::vector<float>(v, 0.0f));
        m_out_ptrs.clear();
        for (auto& b : m_out_bufs) {
            m_out_ptrs.push_back(b.data());
        }
        m_gain_smooth = static_cast<float>(static_cast<double>(gain));
    }

    /// Max calls this (per outlet) to learn the channel count. Signature is
    /// long(t_object*, long); the t_object is the min wrapper instance.
    static long mc_outputs(minwrap<ambitap_room>* self, long /* outlet_index */) {
        return self->m_min_object.m_channel_count;
    }
};

MIN_EXTERNAL(ambitap_room);
