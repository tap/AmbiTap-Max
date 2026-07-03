/// @file
/// ambitap.binaural~ — decode a higher-order ambisonics bus to binaural stereo
/// via SH-domain HRTF convolution (built-in MIT KEMAR, orders 1-5, or a
/// user-supplied SOFA file via the `sofa` attribute), with internal
/// head-tracking.
///
/// Order is a creation argument (default 1, capped at 5 by the built-in HRTF).
/// Input is one multichannel signal of (order+1)^2 channels; output is two
/// signals (left, right). DSP lives in ambitap::dsp::binaural_renderer: a
/// partitioned-FFT convolver bank (AmbiTap::fft) plus an async head-tracking
/// SH-rotation. Convolvers are (re)allocated for the host's signal vector size
/// and sample rate in `dspsetup`; the audio path is wait-free. SOFA loading
/// happens on the control thread: measurements are projected onto the SH basis
/// at this object's order (decompose_sh) and resampled to the host rate.

#include "c74_min.h"

#include "ambitap/dsp/binaural_renderer.h"
#ifdef AMBITAP_HAS_SOFA
#include "ambitap/math/binaural/resample.h"
#include "ambitap/math/binaural/sofa_reader.h"
#endif

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

using namespace c74::min;

class ambitap_binaural : public object<ambitap_binaural>, public mc_operator<> {
public:
    MIN_DESCRIPTION {"Decode a higher-order ambisonics bus to binaural stereo via SH-domain "
                     "HRTF convolution (built-in MIT KEMAR or a SOFA file, orders 1-5), "
                     "with head-tracking."};
    MIN_TAGS {"audio, ambisonics, spatial, binaural, mc"};
    MIN_AUTHOR {"Timothy Place"};
    MIN_RELATED {"ambitap.encode~, ambitap.rotate~, ambitap.decode~"};

    inlet<>  m_in {this, "(multichannelsignal) HOA bus (order 1-5)"};
    outlet<> m_out_left {this, "(signal) left", "signal"};
    outlet<> m_out_right {this, "(signal) right", "signal"};

    /// First creation argument is the ambisonics order (default 1, max 5).
    explicit ambitap_binaural(const atoms& args = {}) {
        int order = 1;
        if (!args.empty())
            order = std::clamp(static_cast<int>(args[0]), 1, ambitap::builtin_hrtf_order);
        m_renderer = std::make_unique<ambitap::dsp::binaural_renderer>(order);
        m_channels = static_cast<long>(m_renderer->channels());
    }

    attribute<number> volume {
        this, "volume", 1.0,
        description {"Linear output gain (post-convolution)."},
        setter {MIN_FUNCTION {
            const double v = args[0];
            if (m_renderer)
                m_renderer->set_volume(static_cast<float>(v));
            return {v};
        }}
    };

    attribute<symbol> hrtf_dataset {
        this, "hrtf_dataset", "ls",
        description {"Built-in HRTF projection: \"ls\" (default) or \"magls\" (better "
                     "high-frequency localization). Switching rebuilds convolvers — not "
                     "real-time safe, may glitch briefly."},
        setter {MIN_FUNCTION {
            if (m_renderer) {
                const std::string name = args[0];
                using projection = ambitap::dsp::binaural_renderer::hrtf_projection;
                m_renderer->set_projection(name == "magls" ? projection::magls : projection::ls);
            }
            return args;
        }}
    };

    attribute<symbol> sofa {
        this, "sofa", "",
        description {"Path to a SOFA file with HRTF measurements to use instead of the "
                     "built-in KEMAR set (absolute path, or resolvable in the Max search "
                     "path). An empty symbol reverts to the built-in set. Loading projects "
                     "the measurements onto the SH basis at this object's order and rebuilds "
                     "convolvers — not real-time safe, may glitch briefly."},
        setter {MIN_FUNCTION {
#ifdef AMBITAP_HAS_SOFA
            const std::string requested = static_cast<std::string>(args[0]);
            if (requested.empty()) {
                m_sofa.reset();
                if (m_renderer)
                    m_renderer->clear_custom_hrtf();
                return args;
            }
            try {
                auto loaded = std::make_unique<ambitap::hrtf_data>(
                    ambitap::load_sofa(resolve_in_search_path(requested)));
                m_sofa = std::move(loaded);
                apply_sofa_hrtf();    // no-op until dspsetup when not yet prepared
                return args;
            }
            catch (const std::exception& e) {
                cerr << e.what() << endl;
                return {sofa.get()};    // keep the previous value
            }
#else
            if (!static_cast<std::string>(args[0]).empty())
                cerr << "this build has no SOFA support (AMBITAP_ENABLE_SOFA was off)" << endl;
            return args;
#endif
        }}
    };

    attribute<number> yaw {
        this, "yaw", 0.0,
        description {"Head-tracking yaw about +Z (up), radians. Applied first."},
        setter {MIN_FUNCTION {
            const double v = args[0];
            if (m_renderer)
                m_renderer->set_yaw(static_cast<float>(v));
            return {v};
        }}
    };

    attribute<number> pitch {
        this, "pitch", 0.0,
        description {"Head-tracking pitch about +Y (left), radians. Applied second."},
        setter {MIN_FUNCTION {
            const double v = args[0];
            if (m_renderer)
                m_renderer->set_pitch(static_cast<float>(v));
            return {v};
        }}
    };

    attribute<number> roll {
        this, "roll", 0.0,
        description {"Head-tracking roll about +X (front), radians. Applied last."},
        setter {MIN_FUNCTION {
            const double v = args[0];
            if (m_renderer)
                m_renderer->set_roll(static_cast<float>(v));
            return {v};
        }}
    };

    /// Allocate the convolver bank for the host's signal vector size and sample
    /// rate. Called by Max whenever the dsp chain is (re)compiled.
    message<> dspsetup {
        this, "dspsetup",
        MIN_FUNCTION {
            const double sample_rate = args[0];
            const long   vector_size = args[1];
            prepare_renderer(vector_size, sample_rate);
            return {};
        }
    };

    void operator()(audio_bundle input, audio_bundle output) {
        const auto frames = input.frame_count();
        const auto in_ch  = input.channel_count();
        double*    out_l  = output.samples(0);
        double*    out_r  = output.samples(1);

        // Convolvers need a power-of-two vector size, set up in dspsetup. If the
        // current block doesn't match what we prepared, emit silence.
        if (!m_renderer->is_prepared() || frames != m_block_size) {
            for (auto i = 0; i < frames; ++i) {
                out_l[i] = 0.0;
                out_r[i] = 0.0;
            }
            return;
        }

        // Gather the HOA channels (double -> float, zero-padded to (order+1)^2).
        for (long c = 0; c < m_channels; ++c) {
            float* dst = m_in_buffers[static_cast<size_t>(c)].data();
            if (c < in_ch) {
                const double* src = input.samples(c);
                for (auto i = 0; i < frames; ++i)
                    dst[i] = static_cast<float>(src[i]);
            }
            else {
                for (auto i = 0; i < frames; ++i)
                    dst[i] = 0.0f;
            }
        }

        m_renderer->process(m_in_ptrs.data(), m_left_buf.data(), m_right_buf.data(),
                            static_cast<size_t>(frames));

        for (auto i = 0; i < frames; ++i) {
            out_l[i] = static_cast<double>(m_left_buf[i]);
            out_r[i] = static_cast<double>(m_right_buf[i]);
        }
    }

private:
    std::unique_ptr<ambitap::dsp::binaural_renderer> m_renderer;
    long                                             m_channels {4};
    long                                             m_block_size {0};
    std::vector<std::vector<float>>                  m_in_buffers;  // [channel][block]
    std::vector<const float*>                        m_in_ptrs;     // into m_in_buffers
    std::vector<float>                               m_left_buf;
    std::vector<float>                               m_right_buf;
#ifdef AMBITAP_HAS_SOFA
    std::unique_ptr<ambitap::hrtf_data>              m_sofa;    // raw measurements, file rate

    // Cap the decomposed FIR length: bounds convolver cost against
    // pathologically long measurement files.
    static constexpr size_t k_max_sofa_taps = 2048;

    /// Resolve a filename through the Max search path to an absolute system
    /// path; a name that cannot be located passes through unchanged (it may be
    /// an absolute path already).
    static std::string resolve_in_search_path(const std::string& name) {
        char filename[c74::max::MAX_PATH_CHARS];
        std::snprintf(filename, sizeof(filename), "%s", name.c_str());
        short             path_id {};
        c74::max::t_fourcc type {};
        if (c74::max::locatefile_extended(filename, &path_id, &type, nullptr, 0) == 0) {
            char absolute[c74::max::MAX_PATH_CHARS];
            if (c74::max::path_toabsolutesystempath(path_id, filename, absolute) == 0)
                return absolute;
        }
        return name;
    }

    /// Project the loaded SOFA measurements onto the SH basis at this object's
    /// order, resample the decomposed FIRs to the host rate, and hand them to
    /// the renderer. No-op when nothing is loaded or DSP is not yet prepared
    /// (dspsetup calls back in here once the host rate is known).
    void apply_sofa_hrtf() {
        if (!m_sofa || !m_renderer || !m_renderer->is_prepared())
            return;
        try {
            std::vector<std::vector<float>> left, right;
            m_sofa->decompose_sh(m_renderer->order(), k_max_sofa_taps, left, right);
            const float host_rate = m_renderer->sample_rate();
            if (m_sofa->sample_rate != host_rate) {
                for (auto* ear : {&left, &right}) {
                    for (auto& fir : *ear)
                        fir = ambitap::resample_fir(fir.data(), fir.size(), m_sofa->sample_rate,
                                                    host_rate);
                }
            }
            m_renderer->set_custom_hrtf(std::move(left), std::move(right));
        }
        catch (const std::exception& e) {
            // e.g. a measurement grid that cannot support this order: report,
            // drop the set, and stay on the built-in HRTFs.
            cerr << e.what() << endl;
            m_sofa.reset();
            m_renderer->clear_custom_hrtf();
        }
    }
#else
    void apply_sofa_hrtf() {}
#endif

    void prepare_renderer(long vector_size, double sample_rate) {
        const bool valid = (vector_size >= 4) && ((vector_size & (vector_size - 1)) == 0);
        if (!valid) {
            m_block_size = 0;    // unsupported vector size -> stay silent
            return;
        }
        m_block_size = vector_size;
        m_renderer->prepare(static_cast<size_t>(vector_size), static_cast<float>(sample_rate));
        apply_sofa_hrtf();    // custom FIRs are resampled per host rate; re-derive

        const auto n = static_cast<size_t>(m_channels);
        const auto v = static_cast<size_t>(vector_size);
        m_in_buffers.assign(n, std::vector<float>(v, 0.0f));
        m_in_ptrs.clear();
        m_in_ptrs.reserve(n);
        for (auto& buf : m_in_buffers)
            m_in_ptrs.push_back(buf.data());
        m_left_buf.assign(v, 0.0f);
        m_right_buf.assign(v, 0.0f);
    }
};

MIN_EXTERNAL(ambitap_binaural);
