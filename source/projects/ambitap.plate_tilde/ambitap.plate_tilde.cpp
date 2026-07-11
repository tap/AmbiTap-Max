/// @file
/// ambitap.plate~ — multichannel plate reverb after Dattorro (1997), the
/// plate-class tank "in the Griesinger style", generalized for N inputs and
/// M outputs.
///
/// Creation args: <inputs> <outputs> <branches>. Inputs (default 2) and
/// outputs (default 2) each go up to 64 and are fixed at construction — one
/// multichannel signal in, one out. Branches (default 4, range 2..8) sets the
/// tank width: 2 is Dattorro's original stereo figure-8; more branches add
/// tail density and decorrelation. Input channels beyond <inputs> are
/// ignored; missing ones are treated as silence.
///
/// DSP lives in ambitap::dsp::plate (header-only, wait-free process path, no
/// worker thread). Every attribute is smoothed/click-free in the kernel. The
/// kernel is 100% wet; this object adds the equal-power dry/wet `mix`, with
/// dry mapped channel-wise over the first min(inputs, outputs) channels.
// SPDX-License-Identifier: MIT
// Copyright 2026 Timothy Place.

#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>

#include "ambitap/dsp/plate.h"
#include "c74_min.h"

using namespace c74::min;

class ambitap_plate : public object<ambitap_plate>, public mc_operator<> {
  public:
    MIN_DESCRIPTION{"Multichannel plate reverb (Dattorro/Griesinger tank). Creation args: "
                    "<inputs> <outputs> <branches>. One multichannel signal in (N channels), "
                    "one out (M channels); channel counts are fixed at construction. Branches "
                    "(2..8) sets the tank width — 2 is the classic stereo figure-8."};
    MIN_TAGS{"audio, reverb, plate, effects, mc"};
    MIN_AUTHOR{"Timothy Place"};
    MIN_RELATED{"ambitap.room~, ambitap.encode~, ambitap.decode~"};

    inlet<>  m_in{this, "(multichannelsignal) inputs to reverberate"};
    outlet<> m_out{this, "(multichannelsignal) reverb outputs", "signal"};

  private:
    // State lives ABOVE the attributes on purpose: min-api attribute
    // construction invokes the custom setter with the default value, and
    // members are initialized in declaration order — everything a setter
    // touches must already be alive.

    /// One-pole coefficient of the per-sample dry/wet slew (~5 ms at 48 kHz).
    static constexpr float k_mix_slew = 1.0f / 256.0f;

    std::unique_ptr<ambitap::dsp::plate> m_plate;
    long                                 m_in_channels{2};
    long                                 m_out_channels{2};
    long                                 m_block_size{0};
    float                                m_wet_smooth{1.0f};
    float                                m_dry_smooth{0.0f};
    std::vector<std::vector<float>>      m_in_bufs;  // [channel][block]
    std::vector<std::vector<float>>      m_out_bufs; // [channel][block]
    std::vector<const float*>            m_in_ptrs;  // into m_in_bufs
    std::vector<float*>                  m_out_ptrs; // into m_out_bufs

  public:
    /// Creation args: <inputs> <outputs> <branches>.
    explicit ambitap_plate(const atoms& args = {}) {
        int inputs   = 2;
        int outputs  = 2;
        int branches = 4;
        if (args.size() >= 1) {
            inputs = std::clamp(static_cast<int>(args[0]), 1, ambitap::dsp::k_plate_max_inputs);
        }
        if (args.size() >= 2) {
            outputs = std::clamp(static_cast<int>(args[1]), 1, ambitap::dsp::k_plate_max_outputs);
        }
        if (args.size() >= 3) {
            branches = std::clamp(static_cast<int>(args[2]), ambitap::dsp::k_plate_min_branches,
                                  ambitap::dsp::k_plate_max_branches);
        }

        m_plate        = std::make_unique<ambitap::dsp::plate>(inputs, outputs, branches);
        m_in_channels  = static_cast<long>(m_plate->inputs());
        m_out_channels = static_cast<long>(m_plate->outputs());
    }

    // Attribute defaults mirror dsp::plate's defaults (Dattorro's table 1
    // values); the setters run before m_plate exists during construction, so
    // the two default sets must match.

    attribute<number> decay{this, "decay", 0.5,
                            description{"Tail decay per tank pass (0..1). 1 = freeze: the tank recirculates "
                                        "losslessly (damping still bleeds energy)."},
                            setter{MIN_FUNCTION{const double v = args[0];
    if (m_plate) {
        m_plate->set_decay(static_cast<float>(v));
    }
    return {v};
}
}
}
;

attribute<number> damping{this, "damping", 0.0005,
                          description{"In-loop high-frequency damping (0..1). 0 = bright/undamped; larger "
                                      "values darken the tail faster."},
                          setter{MIN_FUNCTION{const double v = args[0];
if (m_plate) {
    m_plate->set_damping(static_cast<float>(v));
}
return {v};
}
}
}
;

attribute<number> bandwidth{this, "bandwidth", 0.9995,
                            description{"Input band-limiting one-pole (0..1). 1 = full bandwidth into the "
                                        "tank; lower values pre-darken the input."},
                            setter{MIN_FUNCTION{const double v = args[0];
if (m_plate) {
    m_plate->set_bandwidth(static_cast<float>(v));
}
return {v};
}
}
}
;

attribute<number> diffusion{this, "diffusion", 1.0,
                            description{"Master diffusion scale (0..1.3) applied to the paper's four "
                                        "allpass coefficients (input 0.75/0.625, decay 0.70/0.50). "
                                        "1 = Dattorro's values; lower is grainier, higher is denser."},
                            setter{MIN_FUNCTION{const double v = std::clamp(static_cast<double>(args[0]), 0.0, 1.3);
if (m_plate) {
    const auto d = static_cast<float>(v);
    m_plate->set_input_diffusion_1(0.750f * d);
    m_plate->set_input_diffusion_2(0.625f * d);
    m_plate->set_decay_diffusion_1(0.70f * d);
    m_plate->set_decay_diffusion_2(0.50f * d);
}
return {v};
}
}
}
;

attribute<number> predelay{this, "predelay", 0.0,
                           description{"Pre-tank delay in milliseconds (0..250). Changes glide (brief "
                                       "pitch shift, like any moving delay)."},
                           setter{MIN_FUNCTION{const double v = args[0];
if (m_plate) {
    m_plate->set_predelay_seconds(static_cast<float>(v) * 0.001f);
}
return {v};
}
}
}
;

attribute<number> moddepth{this, "moddepth", 0.5376,
                           description{"Tank-allpass modulation depth in milliseconds (0..2). The paper's "
                                       "16-sample excursion at 29761 Hz is ~0.54 ms; 0 disables the chorusing."},
                           setter{MIN_FUNCTION{const double v = args[0];
if (m_plate) {
    m_plate->set_mod_depth_ms(static_cast<float>(v));
}
return {v};
}
}
}
;

attribute<number> modrate{this, "modrate", 1.0,
                          description{"Tank-allpass modulation rate in Hz (0..10). Each branch runs a "
                                      "slightly detuned rate so the modulations never phase-lock."},
                          setter{MIN_FUNCTION{const double v = args[0];
if (m_plate) {
    m_plate->set_mod_rate_hz(static_cast<float>(v));
}
return {v};
}
}
}
;

attribute<number> mix{this, "mix", 1.0,
                      description{"Equal-power dry/wet mix (0..1, smoothed per sample). 1 = wet only. Dry "
                                  "is mapped channel-wise over the first min(inputs, outputs) channels; "
                                  "outputs beyond the input count stay wet-only."},
                      setter{MIN_FUNCTION{const double v = args[0];
return {std::clamp(v, 0.0, 1.0)};
}
}
}
;

/// Clear the tank (rings, filter states, LFO phases); parameters keep.
message<> clear{this, "clear", "Clear the reverb tank and all delay state.", MIN_FUNCTION{if (m_plate){m_plate->reset();
}
return {};
}
}
;

/// Allocate the plate's audio state for the host's sample rate and this
/// object's scratch buffers for the signal vector size. Called by Max
/// whenever the dsp chain is (re)compiled.
message<>  dspsetup{this, "dspsetup", MIN_FUNCTION{const double sample_rate = args[0];
const long vector_size = args[1];
prepare_plate(vector_size, sample_rate);
return {};
}
}
;

/// Registered at class-setup so the single signal outlet reports the
/// construction-time output count — i.e. behaves as a multichannel output.
/// min-api does not wrap Max's `multichanneloutputs`, so we add it via raw
/// max-api.
message<> maxclass_setup{this, "maxclass_setup", MIN_FUNCTION{c74::max::t_class* c = args[0];
c74::max::class_addmethod(c, reinterpret_cast<c74::max::method>(mc_outputs), "multichanneloutputs", c74::max::A_CANT,
                          0);
return {};
}
}
;

void operator()(audio_bundle input, audio_bundle output) {
    const auto frames = input.frame_count();
    const auto in_ch  = input.channel_count();
    const auto out_ch = output.channel_count();

    // The scratch buffers are sized in dspsetup; emit silence until then
    // (never allocate on the audio thread).
    if (!m_plate->is_prepared() || frames > m_block_size) {
        for (long c = 0; c < out_ch; ++c) {
            double* out = output.samples(c);
            for (auto i = 0; i < frames; ++i) {
                out[i] = 0.0;
            }
        }
        return;
    }

    for (long c = 0; c < m_in_channels; ++c) {
        float* dst = m_in_bufs[static_cast<size_t>(c)].data();
        if (c < in_ch) {
            const double* src = input.samples(c);
            for (auto i = 0; i < frames; ++i) {
                dst[i] = static_cast<float>(src[i]);
            }
        }
        else {
            std::fill_n(dst, static_cast<size_t>(frames), 0.0f);
        }
    }

    m_plate->process(m_in_ptrs.data(), m_out_ptrs.data(), static_cast<size_t>(frames));

    // Equal-power dry/wet, slewed per sample. Dry exists only where an input
    // channel maps onto the same-numbered output channel.
    const auto  mix_target = static_cast<float>(static_cast<double>(mix));
    const float wet_target = std::sin(mix_target * 1.5707963f);
    const float dry_target = std::cos(mix_target * 1.5707963f);

    for (long c = 0; c < out_ch; ++c) {
        double* out = output.samples(c);
        if (c < m_out_channels) {
            const float* wet_src = m_out_bufs[static_cast<size_t>(c)].data();
            const float* dry_src = (c < m_in_channels) ? m_in_bufs[static_cast<size_t>(c)].data() : nullptr;
            float        wet     = m_wet_smooth;
            float        dry     = m_dry_smooth;
            for (auto i = 0; i < frames; ++i) {
                wet += (wet_target - wet) * k_mix_slew;
                dry += (dry_target - dry) * k_mix_slew;
                const float d = dry_src ? dry_src[i] * dry : 0.0f;
                out[i]        = static_cast<double>(wet_src[i] * wet + d);
            }
            if (c == m_out_channels - 1) {
                m_wet_smooth = wet;
                m_dry_smooth = dry;
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
void prepare_plate(long vector_size, double sample_rate) {
    if (vector_size < 1) {
        m_block_size = 0; // stay silent
        return;
    }
    m_block_size = vector_size;
    m_plate->prepare(static_cast<float>(sample_rate));

    const auto v = static_cast<size_t>(vector_size);
    m_in_bufs.assign(static_cast<size_t>(m_in_channels), std::vector<float>(v, 0.0f));
    m_out_bufs.assign(static_cast<size_t>(m_out_channels), std::vector<float>(v, 0.0f));
    m_in_ptrs.clear();
    m_out_ptrs.clear();
    for (auto& b : m_in_bufs) {
        m_in_ptrs.push_back(b.data());
    }
    for (auto& b : m_out_bufs) {
        m_out_ptrs.push_back(b.data());
    }

    const auto mix_now = static_cast<float>(static_cast<double>(mix));
    m_wet_smooth       = std::sin(mix_now * 1.5707963f);
    m_dry_smooth       = std::cos(mix_now * 1.5707963f);
}

/// Max calls this (per outlet) to learn the channel count. Signature is
/// long(t_object*, long); the t_object is the min wrapper instance.
static long mc_outputs(minwrap<ambitap_plate>* self, long /* outlet_index */) {
    return self->m_min_object.m_out_channels;
}
}
;

MIN_EXTERNAL(ambitap_plate);
