// Is float32 + vDSP actually worth a precision change in AmbiTap?
//
// The Max externals use tap::ambi::partitioned_convolver, which is
// basic_partitioned_convolver<double, real_fft> — and double is ALWAYS Ooura,
// so vDSP is compiled in but never called. Moving to vDSP means moving to
// float32, which is a numerics decision, so measure the win before paying for
// it.
//
// The comparison decomposes deliberately into two steps, because they have very
// different costs to adopt:
//
//   double + Ooura   what ships today
//   float  + Ooura   the precision change alone (no backend dependency at all)
//   float  + vDSP    the precision change plus Apple's backend
//
// Build twice, with and without TAP_DSP_FFT_ACCELERATE, to get the third row.
// If most of the win is in the middle row, we can have it without depending on
// vDSP's documented latitude at all — which would be the best outcome
// available, and is not something the "use vDSP" framing would have surfaced.
//
// The convolver's public interface is float in/out either way, so this measures
// exactly what the externals would see.

#include <chrono>
#include <cstdio>
#include <string>
#include <vector>

#include "ambitap/math/binaural/convolution.h"

namespace {

    std::vector<float> noise(size_t n, unsigned seed) {
        std::vector<float> v(n);
        unsigned           s = seed;
        for (auto& x : v) {
            s ^= s << 13;
            s ^= s >> 17;
            s ^= s << 5;
            x = (static_cast<float>(s) / 2147483648.0f - 1.0f) * 0.1f;
        }
        return v;
    }

    template <typename Convolver>
    double ns_per_block(size_t block, size_t ir_len, int blocks) {
        const auto ir = noise(ir_len, 0xC0FFEEu);
        Convolver  c(block, ir.data(), ir.size());

        const auto         in = noise(block, 0x1234u);
        std::vector<float> out(block, 0.0f);

        for (int i = 0; i < 64; ++i) { // warm up caches and any lazy init
            c.process(in.data(), out.data());
        }

        const auto t0 = std::chrono::steady_clock::now();
        for (int i = 0; i < blocks; ++i) {
            c.process(in.data(), out.data());
        }
        const auto t1 = std::chrono::steady_clock::now();

        // Touch the output so the loop cannot be optimized away.
        double sink = 0.0;
        for (const float v : out) {
            sink += v;
        }
        if (sink == 12345.678) {
            std::printf("");
        }

        return std::chrono::duration<double, std::nano>(t1 - t0).count() / blocks;
    }

} // namespace

int main() {
#if defined(TAP_DSP_FFT_ACCELERATE)
    const char* float_backend = "vDSP";
#else
    const char* float_backend = "Ooura";
#endif
    std::printf("float32 backend in this build: %s   (double is always Ooura)\n\n", float_backend);
    std::printf("%-8s %-9s %14s %14s %9s\n", "block", "ir_taps", "double+Ooura", "float+", "speedup");

    // 128 taps  — ambitap.panbin~ / xtc~ (the comment in panbin cites 128).
    // 8192      — a short room impulse.
    // 32768     — a long room impulse, where FFT cost dominates.
    for (const size_t ir_len : {size_t{128}, size_t{8192}, size_t{32768}}) {
        for (const size_t block : {size_t{64}, size_t{128}, size_t{256}}) {
            const int blocks = ir_len > 16384 ? 2000 : 20000;

            const double d = ns_per_block<tap::ambi::partitioned_convolver>(block, ir_len, blocks);
            const double f = ns_per_block<tap::ambi::partitioned_convolver32>(block, ir_len, blocks);

            std::printf("%-8zu %-9zu %14.1f %14.1f %8.2fx\n", block, ir_len, d, f, d / f);
        }
    }
    return 0;
}
