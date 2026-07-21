/// @file
/// ambitap.panbin~ — direct binaural panner: pan a mono source to stereo with
/// a per-source HRTF at (azimuth, elevation), WITHOUT an ambisonic bus. Unlike
/// encode~ → binaural~ there is no order-limited spatial blur: the source is
/// convolved with the HRIR reconstructed exactly at its direction.
///
/// Reuse path: this object does what binaural_renderer::probe_response does to
/// reconstruct an HRIR at an arbitrary direction — evaluate the SH basis at
/// (azimuth, elevation) with tap::ambi::evaluate_sh and sum the built-in KEMAR
/// SH-domain FIRs (hrtf_data.h, order 5 = the full built-in resolution)
/// weighted by those coefficients — but keeps the time-domain result instead
/// of probe_response's magnitude spectrum, resamples it to the host rate with
/// tap::ambi::resample_fir, and feeds one tap::ambi::partitioned_convolver per
/// ear. A whole binaural_renderer is NOT instantiated: probe_response returns
/// only dB magnitudes (useless for convolution), and the renderer would drag
/// in a 36-channel convolver bank and a head-tracking worker thread that this
/// object would never use. Everything direction- and convolution-related is
/// genuine library reuse; only the glue is new.
///
/// Direction changes are click-free. Azimuth/elevation setters run on the
/// control thread, where they rebuild the two-convolver pair (allocation is
/// fine there) and publish it through a lock-free single-slot handoff. The
/// perform routine adopts the new pair at a block boundary, runs old and new
/// in parallel for that one block while crossfading linearly, then parks the
/// old pair in a trash slot that the control thread reaps on its next rebuild
/// (the audio thread never allocates or frees). This is affordable here where
/// it was not for binaural~'s `hrtf_dataset` swap: one source is 2 short
/// convolvers (128 taps), not a 36-channel bank, so the double-render block
/// costs almost nothing. The perform routine is wait-free and allocation-free
/// and emits silence until dspsetup has prepared the convolvers.
// SPDX-License-Identifier: MIT
// Copyright 2025-2026 Timothy Place.

#include <algorithm>
#include <atomic>
#include <memory>
#include <mutex>
#include <vector>

#include "ambitap/math/binaural/convolution.h"
#include "ambitap/math/binaural/hrtf_data.h"
#include "ambitap/math/binaural/resample.h"
#include "ambitap/math/core/spherical_harmonics.h"
#include "c74_min.h"

using namespace c74::min;

class ambitap_panbin : public object<ambitap_panbin>, public vector_operator<> {
    /// One HRIR-loaded convolver per ear. Built on the control thread; used
    /// (and only used) on the audio thread after ownership is handed over.
    struct convolver_pair {
        tap::ambi::partitioned_convolver left;
        tap::ambi::partitioned_convolver right;

        convolver_pair(size_t block_size, const std::vector<float>& left_ir, const std::vector<float>& right_ir)
            : left(block_size, left_ir.data(), left_ir.size())
            , right(block_size, right_ir.data(), right_ir.size()) {}
    };

    // State lives ABOVE the attributes on purpose: min-api attribute
    // construction invokes the custom setter with the default value, and
    // members are initialized in declaration order — everything a setter
    // touches must already be alive.

    // Control-side state (attribute setters may arrive on both the main and
    // the scheduler thread; the mutex serializes them — the audio thread
    // never takes it).
    std::mutex m_control_mutex;
    double     m_azimuth_value{0.0};
    double     m_elevation_value{0.0};
    long       m_block_size{0};
    double     m_sample_rate{0.0};

    // Control -> audio handoff (freshly built pair awaiting adoption) and
    // audio -> control return path (retired pair awaiting deletion).
    std::atomic<convolver_pair*> m_pending{nullptr};
    std::atomic<convolver_pair*> m_trash{nullptr};

    // Gain target: written by the control thread, ramped to by the audio
    // thread (the binaural_core volume pattern).
    std::atomic<float> m_gain{1.0f};

    // Audio-thread-only state.
    convolver_pair*    m_active{nullptr};
    float              m_gain_current{1.0f};
    std::vector<float> m_in_buf;
    std::vector<float> m_left_buf;
    std::vector<float> m_right_buf;
    std::vector<float> m_fade_l;
    std::vector<float> m_fade_r;

  public:
    MIN_DESCRIPTION{"Pan a mono source directly to binaural stereo with a per-source HRTF "
                    "(built-in MIT KEMAR) at an arbitrary azimuth/elevation — no ambisonic "
                    "bus, no order-limited blur. Direction changes crossfade click-free."};
    MIN_TAGS{"audio, spatial, binaural, panning"};
    MIN_AUTHOR{"Timothy Place"};
    MIN_RELATED{"ambitap.encode~, ambitap.binaural~, ambitap.vmic~"};

    inlet<>  m_in{this, "(signal) mono source"};
    outlet<> m_out_left{this, "(signal) left", "signal"};
    outlet<> m_out_right{this, "(signal) right", "signal"};

    ~ambitap_panbin() {
        // DSP is torn down before the object is freed; every live pair is in
        // exactly one of these three places.
        delete m_pending.exchange(nullptr);
        delete m_trash.exchange(nullptr);
        delete m_active;
    }

    attribute<number> azimuth{this, "azimuth", 0.0,
                              description{"Source azimuth in radians. 0 = front, pi/2 = left, pi = behind."},
                              setter{MIN_FUNCTION{
                                  const double value = args[0];
                                  set_direction(value, m_elevation_value);
                                  return {value};
                              }}};

    attribute<number> elevation{this, "elevation", 0.0,
                                description{"Source elevation in radians. 0 = horizon, pi/2 = zenith."},
                                setter{MIN_FUNCTION{
                                    const double value = args[0];
                                    set_direction(m_azimuth_value, value);
                                    return {value};
                                }}};

    attribute<number> gain{this, "gain", 1.0, description{"Linear output gain (post-convolution)."},
                           setter{MIN_FUNCTION{
                               const double value = args[0];
                               m_gain.store(static_cast<float>(value), std::memory_order_relaxed);
                               return {value};
                           }}};

    /// Build the convolvers for the host's signal vector size and sample rate.
    /// Called by Max whenever the dsp chain is (re)compiled — audio is not
    /// running, so we can rebuild everything synchronously.
    message<> dspsetup{this, "dspsetup",
                       MIN_FUNCTION{
                           const double sample_rate = args[0];
                           const long   vector_size = args[1];
                           prepare(vector_size, sample_rate);
                           return {};
                       }};

    void operator()(audio_bundle input, audio_bundle output) {
        const auto frames = input.frame_count();
        double*    out_l  = output.samples(0);
        double*    out_r  = output.samples(1);

        // Convolvers need a power-of-two vector size, set up in dspsetup. If
        // the current block doesn't match what we prepared, emit silence.
        if (m_block_size == 0 || frames != m_block_size) {
            for (auto i = 0; i < frames; ++i) {
                out_l[i] = 0.0;
                out_r[i] = 0.0;
            }
            return;
        }

        const double* in = input.samples(0);
        for (auto i = 0; i < frames; ++i) {
            m_in_buf[i] = static_cast<float>(in[i]);
        }

        // Adopt a newly published direction, but only when the trash slot is
        // free to receive the pair we would retire (the control thread reaps
        // it; the audio thread never frees). A full slot just defers the
        // switch to a later block.
        convolver_pair* incoming = nullptr;
        if (m_trash.load(std::memory_order_relaxed) == nullptr) {
            incoming = m_pending.exchange(nullptr, std::memory_order_acq_rel);
        }

        if (incoming && m_active) {
            // Crossfade: render the block through both the old and the new
            // pair, ramp between them, then park the old pair for reaping.
            m_active->left.process(m_in_buf.data(), m_fade_l.data());
            m_active->right.process(m_in_buf.data(), m_fade_r.data());
            incoming->left.process(m_in_buf.data(), m_left_buf.data());
            incoming->right.process(m_in_buf.data(), m_right_buf.data());

            const float step = 1.0f / static_cast<float>(frames);
            float       w    = 0.0f;
            for (auto i = 0; i < frames; ++i) {
                w += step;
                m_left_buf[i]  = m_fade_l[i] + w * (m_left_buf[i] - m_fade_l[i]);
                m_right_buf[i] = m_fade_r[i] + w * (m_right_buf[i] - m_fade_r[i]);
            }
            m_trash.store(m_active, std::memory_order_release);
            m_active = incoming;
        }
        else {
            if (incoming) {
                m_active = incoming; // first-ever pair: nothing to fade from
            }
            if (!m_active) {
                for (auto i = 0; i < frames; ++i) {
                    out_l[i] = 0.0;
                    out_r[i] = 0.0;
                }
                return;
            }
            m_active->left.process(m_in_buf.data(), m_left_buf.data());
            m_active->right.process(m_in_buf.data(), m_right_buf.data());
        }

        // Gain: linear ramp from the previous value to the target across this
        // block (click-free, race-free) — the binaural_core volume pattern.
        const float target = m_gain.load(std::memory_order_relaxed);
        float       g      = m_gain_current;
        const float g_step = (target - g) / static_cast<float>(frames);
        for (auto i = 0; i < frames; ++i) {
            g += g_step;
            out_l[i] = static_cast<double>(m_left_buf[i] * g);
            out_r[i] = static_cast<double>(m_right_buf[i] * g);
        }
        m_gain_current = target;
    }

  private:
    /// Reconstruct the per-ear HRIRs at the current direction — the same sum
    /// binaural_renderer::probe_response performs (SH basis at the direction
    /// weighting the built-in order-5 SH-domain FIRs) — resampled to the host
    /// rate, and wrap them in a fresh convolver pair. Control thread only.
    std::unique_ptr<convolver_pair> build_pair() {
        float sh[tap::ambi::k_max_channel_count];
        tap::ambi::evaluate_sh(tap::ambi::builtin_hrtf_order, static_cast<float>(m_azimuth_value),
                             static_cast<float>(m_elevation_value), sh);

        std::vector<float> left(tap::ambi::builtin_hrtf_length, 0.0f);
        std::vector<float> right(tap::ambi::builtin_hrtf_length, 0.0f);
        for (size_t ch = 0; ch < tap::ambi::builtin_hrtf_channels; ++ch) {
            for (size_t t = 0; t < tap::ambi::builtin_hrtf_length; ++t) {
                left[t] += sh[ch] * tap::ambi::builtin_hrtf_left[ch][t];
                right[t] += sh[ch] * tap::ambi::builtin_hrtf_right[ch][t];
            }
        }

        const auto host_rate = static_cast<float>(m_sample_rate);
        if (host_rate != tap::ambi::builtin_hrtf_sample_rate) {
            left  = tap::ambi::resample_fir(left.data(), left.size(), tap::ambi::builtin_hrtf_sample_rate, host_rate);
            right = tap::ambi::resample_fir(right.data(), right.size(), tap::ambi::builtin_hrtf_sample_rate, host_rate);
        }

        return std::make_unique<convolver_pair>(static_cast<size_t>(m_block_size), left, right);
    }

    /// Store the new direction and, when prepared, publish a freshly built
    /// convolver pair for the audio thread to crossfade to.
    void set_direction(double azimuth_radians, double elevation_radians) {
        std::lock_guard<std::mutex> lock(m_control_mutex);
        m_azimuth_value   = azimuth_radians;
        m_elevation_value = elevation_radians;
        if (m_block_size == 0) {
            return; // not prepared yet; dspsetup builds the first pair
        }

        delete m_trash.exchange(nullptr, std::memory_order_acq_rel); // reap
        // A still-unadopted previous pending pair comes back to us here and is
        // deleted — the audio thread only ever sees the newest direction.
        delete m_pending.exchange(build_pair().release(), std::memory_order_acq_rel);
    }

    void prepare(long vector_size, double sample_rate) {
        std::lock_guard<std::mutex> lock(m_control_mutex);

        // Audio is stopped during dspsetup: reclaim every pair synchronously.
        delete m_pending.exchange(nullptr);
        delete m_trash.exchange(nullptr);
        delete m_active;
        m_active = nullptr;

        const bool valid = (vector_size >= 4) && ((vector_size & (vector_size - 1)) == 0);
        if (!valid) {
            m_block_size = 0; // unsupported vector size -> stay silent
            return;
        }
        m_block_size  = vector_size;
        m_sample_rate = sample_rate;

        const auto v = static_cast<size_t>(vector_size);
        m_in_buf.assign(v, 0.0f);
        m_left_buf.assign(v, 0.0f);
        m_right_buf.assign(v, 0.0f);
        m_fade_l.assign(v, 0.0f);
        m_fade_r.assign(v, 0.0f);

        m_active = build_pair().release();
    }
};

MIN_EXTERNAL(ambitap_panbin);
