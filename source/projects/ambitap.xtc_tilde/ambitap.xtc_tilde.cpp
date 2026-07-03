/// @file
/// ambitap.xtc~ — transaural / crosstalk cancellation: play stereo or binaural
/// program over TWO loudspeakers so each ear hears (mostly) only its own
/// channel. The 2x2 filter matrix is a regularized frequency-domain inverse of
/// the KEMAR speaker→ear plant for a stated symmetric geometry (span,
/// distance), designed entirely by dsp::xtc in the AmbiTap library — v1 is
/// computed-plant presets only (no measured-plant loading). The design meets
/// the numeric gates X1–X6 of docs/PERCEPTUAL-VERIFICATION.md (verified in the
/// library's test suite); the `bypass` attribute is the A/B leg the listening
/// protocol requires.
///
/// Threading follows the ambitap.panbin~ pattern, widened from 2 filters to 4
/// (2-in/2-out). Attribute setters run on the control thread, where they
/// redesign the FIRs (dsp::xtc::set_* — allocation and FFTs are fine there)
/// and publish a freshly built quad of partitioned convolvers through a
/// lock-free single-slot handoff. The perform routine adopts the new quad at a
/// block boundary, runs old and new in parallel for that one block while
/// crossfading linearly, then parks the old quad in a trash slot that the
/// control thread reaps on its next rebuild — the audio thread never allocates
/// or frees. Bypass ramps over one block so A/B toggling doesn't click. The
/// perform routine is wait-free and allocation-free and emits silence until
/// dspsetup has prepared the convolvers.
///
/// Latency: dsp::xtc realizes the (intrinsically non-causal) inverse with a
/// modeling delay of half the FIR length — 512 samples — on top of the host
/// vector; bypass is delay-compensated NOT (it is plain passthrough), since
/// the listening protocol compares loudness-matched program, not sample
/// alignment. The shipped filters carry the X5 makeup attenuation (~-12 dB),
/// so expect the cancelled path noticeably quieter than bypass: match levels
/// upstream when running the protocol.

#include "c74_min.h"

#include "ambitap/dsp/xtc.h"

#include <algorithm>
#include <atomic>
#include <memory>
#include <mutex>
#include <vector>

using namespace c74::min;

class ambitap_xtc : public object<ambitap_xtc>, public vector_operator<> {
    /// The four FIR convolvers of one design: speaker feed = row, program
    /// input = column. Built on the control thread; used (and only used) on
    /// the audio thread after ownership is handed over.
    struct convolver_quad {
        ambitap::partitioned_convolver ll;    // left speaker  <- left input
        ambitap::partitioned_convolver lr;    // left speaker  <- right input
        ambitap::partitioned_convolver rl;    // right speaker <- left input
        ambitap::partitioned_convolver rr;    // right speaker <- right input

        convolver_quad(size_t block_size, const ambitap::dsp::xtc& design)
            : ll(block_size, design.fir(0, 0).data(), design.fir(0, 0).size())
            , lr(block_size, design.fir(0, 1).data(), design.fir(0, 1).size())
            , rl(block_size, design.fir(1, 0).data(), design.fir(1, 0).size())
            , rr(block_size, design.fir(1, 1).data(), design.fir(1, 1).size()) {}

        /// out_l/out_r = quad applied to (in_l, in_r); tmp is caller scratch.
        void process(const float* in_l, const float* in_r, float* out_l, float* out_r,
                     float* tmp, long frames) {
            ll.process(in_l, out_l);
            lr.process(in_r, tmp);
            for (long i = 0; i < frames; ++i) out_l[i] += tmp[i];
            rl.process(in_l, out_r);
            rr.process(in_r, tmp);
            for (long i = 0; i < frames; ++i) out_r[i] += tmp[i];
        }
    };

    // State lives ABOVE the attributes on purpose: min-api attribute
    // construction invokes the custom setter with the default value, and
    // members are initialized in declaration order — everything a setter
    // touches must already be alive.

    // Control-side state (attribute setters may arrive on both the main and
    // the scheduler thread; the mutex serializes them — the audio thread
    // never takes it). m_design owns the filter design; its setters redesign
    // synchronously on this thread.
    std::mutex        m_control_mutex;
    ambitap::dsp::xtc m_design;
    long              m_block_size {0};
    double            m_sample_rate {0.0};

    // Control -> audio handoff (freshly built quad awaiting adoption) and
    // audio -> control return path (retired quad awaiting deletion).
    std::atomic<convolver_quad*> m_pending {nullptr};
    std::atomic<convolver_quad*> m_trash {nullptr};

    // Bypass target: written by the control thread, ramped to by the audio
    // thread across one block (click-free A/B).
    std::atomic<float> m_bypass_target {0.0f};

    // Audio-thread-only state.
    convolver_quad*    m_active {nullptr};
    float              m_bypass_current {0.0f};
    std::vector<float> m_in_l, m_in_r;          // float mirrors of the inputs
    std::vector<float> m_wet_l, m_wet_r;        // active-quad render
    std::vector<float> m_fade_l, m_fade_r;      // retiring-quad render
    std::vector<float> m_tmp;                   // convolver mix scratch

public:
    MIN_DESCRIPTION {"Crosstalk cancellation (transaural): play stereo or binaural program "
                     "over two loudspeakers at a known span and distance so each ear hears "
                     "only its own channel. Regularized KEMAR plant inverse (computed-plant "
                     "presets, AmbiTap dsp::xtc); parameter changes crossfade click-free."};
    MIN_TAGS {"audio, spatial, binaural, transaural, loudspeakers"};
    MIN_AUTHOR {"Timothy Place"};
    MIN_RELATED {"ambitap.binaural~, ambitap.panbin~, ambitap.decode~"};

    inlet<>  m_in_left {this, "(signal) left program (binaural or stereo)"};
    inlet<>  m_in_right {this, "(signal) right program (binaural or stereo)"};
    outlet<> m_out_left {this, "(signal) left speaker feed", "signal"};
    outlet<> m_out_right {this, "(signal) right speaker feed", "signal"};

    ~ambitap_xtc() {
        // DSP is torn down before the object is freed; every live quad is in
        // exactly one of these three places.
        delete m_pending.exchange(nullptr);
        delete m_trash.exchange(nullptr);
        delete m_active;
    }

    attribute<number> span {
        this, "span", 20.0,
        description {"Full angle between the loudspeakers in degrees (speakers at ±span/2), "
                     "clamped to [5, 120]. Default 20 — the ±10° desktop geometry the "
                     "verification gates run at."},
        setter {MIN_FUNCTION {
            const double value = args[0];
            std::lock_guard<std::mutex> lock(m_control_mutex);
            m_design.set_span(static_cast<float>(value));
            publish();
            return {static_cast<double>(m_design.span())};
        }}
    };

    attribute<number> distance {
        this, "distance", 1.0,
        description {"Listener-to-speaker distance in meters (>= 0.1). Part of the stated "
                     "geometry; the v1 symmetric far-field design is invariant to it (equal "
                     "path delays and 1/r gains cancel), but state it truthfully — the "
                     "robustness verification displaces the head against it."},
        setter {MIN_FUNCTION {
            const double value = args[0];
            std::lock_guard<std::mutex> lock(m_control_mutex);
            m_design.set_distance(static_cast<float>(value));
            publish();
            return {static_cast<double>(m_design.distance())};
        }}
    };

    attribute<number> regularization {
        this, "regularization", 0.5,
        description {"In-band regularization amount, 0..1 (default 0.5 — the verified "
                     "setting). Scales the cancellation-band beta across ±1 decade: lower "
                     "buys deeper rejection with a hotter, more position-sensitive filter; "
                     "higher trades rejection for robustness and headroom."},
        setter {MIN_FUNCTION {
            const double value = args[0];
            std::lock_guard<std::mutex> lock(m_control_mutex);
            m_design.set_regularization(static_cast<float>(value));
            publish();
            return {static_cast<double>(m_design.regularization())};
        }}
    };

    attribute<bool> bypass {
        this, "bypass", false,
        description {"Pass the input straight through (the A/B reference required by the "
                     "listening protocol). Toggling ramps over one signal vector, so it "
                     "does not click. Note the cancelled path carries the +12 dB-ceiling "
                     "makeup attenuation and 512 samples of filter latency; loudness-match "
                     "upstream for fair comparison."},
        setter {MIN_FUNCTION {
            const bool value = args[0];
            m_bypass_target.store(value ? 1.0f : 0.0f, std::memory_order_relaxed);
            return {value};
        }}
    };

    /// Build the convolvers for the host's signal vector size and sample rate.
    /// Called by Max whenever the dsp chain is (re)compiled — audio is not
    /// running, so we can rebuild everything synchronously.
    message<> dspsetup {
        this, "dspsetup",
        MIN_FUNCTION {
            const double sample_rate = args[0];
            const long   vector_size = args[1];
            prepare(vector_size, sample_rate);
            return {};
        }
    };

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

        // Copy the inputs up front: outputs may alias inputs, and the dry
        // (bypass) mix needs them after the wet render.
        const double* in_l = input.samples(0);
        const double* in_r = input.samples(1);
        for (auto i = 0; i < frames; ++i) {
            m_in_l[i] = static_cast<float>(in_l[i]);
            m_in_r[i] = static_cast<float>(in_r[i]);
        }

        // Adopt a newly published design, but only when the trash slot is
        // free to receive the quad we would retire (the control thread reaps
        // it; the audio thread never frees). A full slot just defers the
        // switch to a later block.
        convolver_quad* incoming = nullptr;
        if (m_trash.load(std::memory_order_relaxed) == nullptr)
            incoming = m_pending.exchange(nullptr, std::memory_order_acq_rel);

        if (incoming && m_active) {
            // Crossfade: render the block through both the old and the new
            // quad, ramp between them, then park the old quad for reaping.
            m_active->process(m_in_l.data(), m_in_r.data(), m_fade_l.data(), m_fade_r.data(),
                              m_tmp.data(), frames);
            incoming->process(m_in_l.data(), m_in_r.data(), m_wet_l.data(), m_wet_r.data(),
                              m_tmp.data(), frames);
            const float step = 1.0f / static_cast<float>(frames);
            float       w    = 0.0f;
            for (auto i = 0; i < frames; ++i) {
                w += step;
                m_wet_l[i] = m_fade_l[i] + w * (m_wet_l[i] - m_fade_l[i]);
                m_wet_r[i] = m_fade_r[i] + w * (m_wet_r[i] - m_fade_r[i]);
            }
            m_trash.store(m_active, std::memory_order_release);
            m_active = incoming;
        }
        else {
            if (incoming)
                m_active = incoming;    // first-ever quad: nothing to fade from
            if (!m_active) {
                for (auto i = 0; i < frames; ++i) {
                    out_l[i] = 0.0;
                    out_r[i] = 0.0;
                }
                return;
            }
            m_active->process(m_in_l.data(), m_in_r.data(), m_wet_l.data(), m_wet_r.data(),
                              m_tmp.data(), frames);
        }

        // Bypass: linear ramp from the previous mix to the target across this
        // block (click-free, race-free). The convolvers rendered above either
        // way, so their history stays warm and un-bypassing is seamless.
        const float target = m_bypass_target.load(std::memory_order_relaxed);
        float       b      = m_bypass_current;
        const float b_step = (target - b) / static_cast<float>(frames);
        for (auto i = 0; i < frames; ++i) {
            b += b_step;
            out_l[i] = static_cast<double>(m_wet_l[i] + b * (m_in_l[i] - m_wet_l[i]));
            out_r[i] = static_cast<double>(m_wet_r[i] + b * (m_in_r[i] - m_wet_r[i]));
        }
        m_bypass_current = target;
    }

private:
    /// Publish a freshly built convolver quad for the audio thread to
    /// crossfade to. Caller holds m_control_mutex; no-op until prepared
    /// (dspsetup builds the first quad).
    void publish() {
        if (m_block_size == 0)
            return;
        delete m_trash.exchange(nullptr, std::memory_order_acq_rel);    // reap
        // A still-unadopted previous pending quad comes back to us here and is
        // deleted — the audio thread only ever sees the newest design.
        auto quad = std::make_unique<convolver_quad>(static_cast<size_t>(m_block_size), m_design);
        delete m_pending.exchange(quad.release(), std::memory_order_acq_rel);
    }

    void prepare(long vector_size, double sample_rate) {
        std::lock_guard<std::mutex> lock(m_control_mutex);

        // Audio is stopped during dspsetup: reclaim every quad synchronously.
        delete m_pending.exchange(nullptr);
        delete m_trash.exchange(nullptr);
        delete m_active;
        m_active = nullptr;

        const bool valid = (vector_size >= 4) && ((vector_size & (vector_size - 1)) == 0);
        if (!valid) {
            m_block_size = 0;    // unsupported vector size -> stay silent
            return;
        }
        m_block_size = vector_size;
        if (sample_rate != m_sample_rate) {
            m_sample_rate = sample_rate;
            m_design.set_sample_rate(static_cast<float>(sample_rate));    // redesigns the FIRs
        }

        const auto v = static_cast<size_t>(vector_size);
        m_in_l.assign(v, 0.0f);
        m_in_r.assign(v, 0.0f);
        m_wet_l.assign(v, 0.0f);
        m_wet_r.assign(v, 0.0f);
        m_fade_l.assign(v, 0.0f);
        m_fade_r.assign(v, 0.0f);
        m_tmp.assign(v, 0.0f);

        m_active = new convolver_quad(v, m_design);
    }
};

MIN_EXTERNAL(ambitap_xtc);
