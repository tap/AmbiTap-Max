# AmbiTap-Max

[![CI](https://github.com/tap/AmbiTap-Max/actions/workflows/ci.yml/badge.svg)](https://github.com/tap/AmbiTap-Max/actions/workflows/ci.yml)

Max/MSP externals for higher-order ambisonics (AmbiX: ACN ordering, SN3D
normalization), built as thin wrappers over the **AmbiTap** library
(`ambitap::dsp` / `ambitap::analysis` processors). A Cycling '74 Min-DevKit
package: one external per folder under `source/projects/`.

## Status

Early scaffold. Objects landed so far (multichannel, order as a creation arg):

- **`ambitap.encode~`** — mono source → higher-order ambisonics. **Order is a
  creation argument** (`ambitap.encode~ 3`); the output is a **single
  multichannel signal** of `(order+1)²` channels (one patch cord for the HOA
  bus). Attributes: `azimuth`, `elevation`, `gain`. Wraps `ambitap::dsp::encoder`.
- **`ambitap.rotate~`** — HOA bus → rotated HOA bus, multichannel in **and** out
  (`(order+1)²` channels each). Attributes: `yaw`, `pitch`, `roll` (Euler,
  Z-Y-X). Wraps `ambitap::dsp::rotator`; the SH-rotation matrix rebuilds on a
  worker thread (`async_rebuilder`) and the audio path reads it wait-free.
- **`ambitap.decode~`** — HOA bus → loudspeaker feeds. Creation args
  `<order> <layout>` (layouts: stereo, quad, surround_5_1, hexagon,
  surround_7_1, cube, octagon, surround_7_1_4); the layout fixes the output
  channel count. MC out (→ `mc.dac~`). Attributes `decoder_type`
  (mode_match / allrad / epad) and `max_re` rebuild the matrix off-thread.
  Wraps `ambitap::dsp::decoder`.
- **`ambitap.binaural~`** — HOA bus → binaural stereo (two outlets) via SH-domain
  HRTF convolution (built-in MIT KEMAR, orders 1–5, or a user SOFA file), with
  internal head-tracking. Attributes: `volume`, `hrtf_dataset` (ls/magls),
  `sofa` (path to a SOFA file — measurements are projected onto the SH basis at
  this object's order and resampled to the host rate; empty reverts to KEMAR),
  `yaw`/`pitch`/`roll`. The convolver bank is allocated for the host vector size
  and sample rate in `dspsetup`. Wraps `ambitap::dsp::binaural_renderer`; links
  `AmbiTap::fft` (Ooura) for the partitioned convolution and libmysofa (fetched
  by the AmbiTap submodule's CMake with `AMBITAP_ENABLE_SOFA=ON`) for SOFA.
- **`ambitap.bed2hoa~`** — channel-based surround bed (5.1 / 7.1 / 7.1.4 / …) →
  HOA, encoding each speaker feed at its canonical direction. Creation args
  `<order> <layout>` (same layout set as `ambitap.decode~`; no LFE — route it
  around the bus). Static encoding matrix; MC in/out.
- **`ambitap.mirror~`** — LR / FB / UD sign-flip mirror (`flip_lr`/`flip_fb`/
  `flip_ud`). MC in/out.
- **`ambitap.format~`** — FuMa ↔ AmbiX conversion (orders 0–3; `direction`). MC in/out.
- **`ambitap.vmic~`** — virtual mic: MC HOA → mono directional extraction
  (`azimuth`/`elevation`/`max_re`).
- **`ambitap.directional~`** — per-direction gain (`azimuth`/`elevation`/`gain`).
  MC in/out.
- **`ambitap.doppler~`** — variable propagation delay (`distance`/
  `speed_of_sound`/`max_distance`). MC in/out.
- **`ambitap.compress~`** — spatial-image-preserving compressor, W-keyed
  (`threshold`/`ratio`/`attack`/`release`/`makeup_gain`). MC in/out.
- **`ambitap.energyvec~`** — active-intensity DOA: MC HOA → x / y / z signals
  (`smoothing_time`).
- **`ambitap.grid~`** — soundfield energy heatmap analysis
  (`analysis::soundfield_grid`): MC HOA passthrough + a `grid <rows> <cols>
  <peak_db> <values...>` list on bang (drive from `qmetro` at display rate),
  the message the AmbiTap UI layer's v8ui heatmap widget consumes (library
  repo, `ui/UI.md`). Attributes: `azimuth_steps`, `smoothing_time` (ms),
  `dynamic_range` (dB).
- **`ambitap.panbin~`** — direct per-source binaural panner: mono + (`azimuth`/
  `elevation`) → stereo through a per-direction HRTF (order-5 KEMAR SH set),
  no ambisonic bus, no order-limited blur. Direction changes crossfade
  click-free via a lock-free convolver-pair handoff; the audio path never
  allocates. Complements encode~ → binaural~ at small source counts.
- **`ambitap.distance~`** — distance cues for an HOA bus: Doppler delay →
  1/r gain (`attenuation` exponent) → air-absorption low-pass → near-field
  compensation (`dsp::nfc`, per-order shelving). Attributes: `distance`,
  `reference_distance`, `attenuation`, `air_absorption`, `speed_of_sound`,
  `max_distance`, `doppler`/`nfc` toggles. MC in/out.
- **`ambitap.xtc~`** — transaural crosstalk cancellation: stereo/binaural →
  two loudspeakers at a known symmetric geometry (`span` degrees, `distance`
  meters), designed per-geometry from the KEMAR plant via `dsp::xtc`
  (regularized 2×2 inversion; gates X1–X6 of the library's
  `docs/PERCEPTUAL-VERIFICATION.md` pass in its test suite). Attributes:
  `span`, `distance`, `regularization`, `bypass` (ramped, for the listening
  protocol's A/B). 512-sample latency; output sits ~12 dB below bypass
  (the gain-ceiling makeup) — loudness-match upstream when comparing.
- **`ambitap.room~`** — shoebox room: mono source → HOA bus carrying direct
  path + image-source early reflections + a 16-line SH-domain FDN tail
  (`dsp::room`, the architecture selected and verified by the library's
  R1–R10 harness). Creation arg `<order>` (max 3). Attributes: `dim_x/y/z`,
  `source_x/y/z`, `listener_x/y/z`, `rt60` (plus `rt60band <hz> <sec>` and
  `reflections <6 floats>` messages), `direct`/`er`/`tail` toggles, `gain`,
  and `absorption` (`fir` — the verified linear-phase filters, default — or
  `iir`, a much cheaper first-order per-line low-pass that trades exact
  mid-band RT60 for a large CPU saving; the tail stays level-calibrated).
  Fixed ~53 ms latency at 48 kHz (injection alignment inherent to the
  verified design; `latency_samples()` exposed for hosts that compensate).

## Layout

```
AmbiTap-Max/
├── CMakeLists.txt              package build (min-devkit convention)
├── package-info.json
├── source/
│   ├── min-api/    → Cycling '74 min-api   (dev: symlink; repo: submodule)
│   ├── min-lib/    → Cycling '74 min-lib   (dev: symlink; repo: submodule)
│   └── projects/<object>/      one external each (.cpp + CMakeLists.txt)
└── externals/                  built .mxo bundles
```

AmbiTap is found at the sibling `../AmbiTap` by default; override with
`-DAmbiTap_ROOT=/path/to/AmbiTap`.

## Build

```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build
# externals land in externals/ (e.g. ambitap.encode_tilde.mxo → object ambitap.encode~)
# v8ui widget bundles land in javascript/ (e.g. ambitap.panner.js for [v8ui])
```

Requires the Cycling '74 min-api/min-lib under `source/` (currently symlinked
from an installed Min-DevKit; will be git submodules once this is a repo) and a
sibling AmbiTap checkout.

The build also requires **node/npm**: the AmbiTap UI layer's v8ui widget
bundles (panner, heatmap, doa, meters, rotation, layout, roomdesigner,
xtcdesigner — sources in the library repo's `ui/`, see its `ui/UI.md`) are
built from `${AmbiTap_ROOT}/ui` with `npm ci` + esbuild at CMake build time
and staged into `javascript/`, which is generated (gitignored), never
committed. Skip with `-DAMBITAP_MAX_BUILD_UI=OFF` if node is unavailable.

To use the objects in Max, make Max see this package — symlink (or copy) the
`AmbiTap-Max` folder into `~/Documents/Max 9/Packages/`:

```bash
ln -s "$PWD" ~/Documents/Max\ 9/Packages/AmbiTap-Max
```

Then the externals load and `help/<object>.maxhelp` opens from each object.

## UI widgets & demo patchers

The v8ui widgets (built into `javascript/`, see Build above) come with
patchers wiring them to the externals — these double as the **in-Max
verification checklist** for the whole UI layer:

- **`patchers/ambitap.ui-tour.maxpat`** — everything at once: two panners →
  `encode~` ×2 → `mc.+~` → `rotate~` (rotation ball) → `grid~` → heatmap,
  `energyvec~` → DOA dot, `mc.peakamp~` → meters, `decode~ 7.1.4` →
  per-speaker layout levels, `binaural~` monitoring, and the OSC
  remote-surface route (`udpreceive 7500` ← the browser dashboard via the
  library repo's `ui/scripts/osc-bridge.mjs`).
- **`patchers/ambitap.roomdesigner.maxpat`** — room designer ↔
  `ambitap.room~` (click~ impulses → binaural monitor; rt60/reflections
  feed both the object and the widget's reflectogram overlay).
- **`patchers/ambitap.xtcdesigner.maxpat`** — xtc designer ↔ `ambitap.xtc~`
  (edits drive the object on release; each change re-requests `dumpfir` so
  the plot shows the running object's designed filters; loudness-matched
  A/B notes included).
- **`help/ambitap.grid~.maxhelp`** — the new analysis external with the
  heatmap widget on its list outlet.

These patch files are hand-authored JSON and, like everything else in this
package, **need in-Max verification** — in particular the `v8ui` box
serialization (re-save from Max once confirmed) and the widget mouse/message
protocols.

## Continuous integration

`.github/workflows/ci.yml` builds all externals universal on macOS and checks
they came out fat. Both this repo and the AmbiTap submodule are public, so the
recursive checkout needs no token.

## Roadmap

The product plan lives in the AmbiTap library repo: **`docs/ROADMAP.md`**
(wrappers + object line). That document is the authority on what gets built
next. Status of this repo against it:

- **Wave 1 (encode / rotate / decode / binaural) — code complete**, plus seven
  further objects (mirror, format, vmic, directional, doppler, compress,
  energyvec). All register MC outputs via the `multichanneloutputs` pattern
  where the output is an HOA bus. **Channel negotiation still needs in-Max
  verification** (load `ambitap.encode~ 5`, confirm 36 channels into an
  `mc.*` object) — none of these objects has been exercised in a running Max.
- **Wave 2 — code complete:** `binaural~` loads user HRTFs via the `sofa`
  attribute (library `sofa_reader` + `decompose_sh`, resampled to the host
  rate), and `ambitap.bed2hoa~` encodes 5.1/7.1/7.1.4 beds into the HOA
  domain. Like Wave 1, still needs in-Max verification.
- **Wave 3 (object line) — code complete.** `panbin~`, `distance~` (library
  `dsp::nfc`), `xtc~` (library `dsp::xtc`, gates X1–X6 green), and `room~`
  (library `dsp::room`, whose C++ render passes the same R1–R10 harness that
  selected its FDN architecture). The perceptual objects (`xtc~`, `room~`)
  still owe the listening pass (bypass rule) from the library's
  `docs/PERCEPTUAL-VERIFICATION.md` before they ship in a release.
- UI: `jit.matrix` soundfield heatmap, JSUI direction picker / polar meter
  (see the portability plan's UI section).

Note the externals compile as **C++20** (AmbiTap requires it); each project's
CMakeLists re-raises `CXX_STANDARD` after `min-posttarget.cmake` pins it to 17.
