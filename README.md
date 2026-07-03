# AmbiTap-Max

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
  HRTF convolution (built-in MIT KEMAR, orders 1–5), with internal head-tracking.
  Attributes: `volume`, `hrtf_dataset` (ls/magls), `yaw`/`pitch`/`roll`. The
  convolver bank is allocated for the host vector size in `dspsetup`. Wraps
  `ambitap::dsp::binaural_renderer`; links `AmbiTap::fft` (Ooura) for the
  partitioned convolution.
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
```

Requires the Cycling '74 min-api/min-lib under `source/` (currently symlinked
from an installed Min-DevKit; will be git submodules once this is a repo) and a
sibling AmbiTap checkout.

To use the objects in Max, make Max see this package — symlink (or copy) the
`AmbiTap-Max` folder into `~/Documents/Max 9/Packages/`:

```bash
ln -s "$PWD" ~/Documents/Max\ 9/Packages/AmbiTap-Max
```

Then the externals load and `help/<object>.maxhelp` opens from each object.

## Continuous integration

`.github/workflows/ci.yml` builds all externals universal on macOS and checks
they came out fat. Because the AmbiTap submodule is a **private** repo, CI needs
a token with read access to it — add a repo secret `AMBITAP_PAT` (a fine-grained
PAT scoped to `tap/AmbiTap`, Contents: Read-only, or a classic `repo`-scope PAT):

```bash
gh secret set AMBITAP_PAT --repo tap/AmbiTap-Max
```

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
- **Wave 2 gaps:** `binaural~` has no SOFA-HRTF loading attribute yet (the
  library's `sofa_reader` + `decompose_sh` make this nearly free); no
  surround-bed → HOA encode mode (`bed2hoa`).
- **Wave 3 (object line):** `panbin~`, `distance~`, `xtc~`, `room~` — not
  started; see the roadmap for scope and the measurement/listening gate on
  the perceptual ones.
- UI: `jit.matrix` soundfield heatmap, JSUI direction picker / polar meter
  (see the portability plan's UI section).

Note the externals compile as **C++20** (AmbiTap requires it); each project's
CMakeLists re-raises `CXX_STANDARD` after `min-posttarget.cmake` pins it to 17.
