# AmbiTap-Max — Max/MSP externals over the AmbiTap ambisonics library.
# Structure follows the Cycling '74 Min-DevKit package convention.

cmake_minimum_required(VERSION 3.19)
project(AmbiTap-Max)

# Raise the default C++ standard for every target. min-api uses std::clamp
# (C++17) and the AmbiTap core needs C++20; AppleClang defaults high enough so
# macOS was fine, but MSVC defaults to C++14, which broke on std::clamp.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

# The Max SDK pulls in <windows.h>, whose `min`/`max` macros break min-api's
# `min::clamp` (and any std::min/std::max). Suppress them (and trim the heavy
# include) for every target added below, including the AmbiTap subdirectory.
if (WIN32)
    add_compile_definitions(NOMINMAX WIN32_LEAN_AND_MEAN)
    # min-devkit builds the externals with the static CRT (/MT). The AmbiTap
    # core (and its libmysofa) use CMP0091-NEW, so they'd otherwise default to
    # the dynamic /MD and clash (LNK4098; binaural~ fails to link). Force the
    # static runtime on the policy-NEW targets so every object matches /MT.
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    if (MSVC)
        # /bigobj: Eigen- and min-api-heavy TUs blow past the COFF section limit.
        add_compile_options(/bigobj)
        # /FIalgorithm: min-api's c74_min_limit.h does `using std::clamp;` without
        #   including <algorithm>; it resolves transitively on AppleClang/libc++
        #   but not on MSVC's STL, so force-include it. C++ ONLY — force-including
        #   a C++ STL header into the C sources (Ooura FFT, libmysofa) trips
        #   "STL1003: expected C++ compiler".
        add_compile_options($<$<COMPILE_LANGUAGE:CXX>:/FIalgorithm>)
    endif ()
endif ()

string(REGEX REPLACE "(.*)/" "" THIS_PACKAGE_NAME "${CMAKE_CURRENT_SOURCE_DIR}")

if (APPLE)
    if (${CMAKE_GENERATOR} MATCHES "Xcode")
        if (${XCODE_VERSION} VERSION_LESS 10)
            message(STATUS "Xcode 10 or higher is required. Please install from the Mac App Store.")
            return ()
        elseif (${XCODE_VERSION} VERSION_GREATER_EQUAL 12)
            set(C74_BUILD_FAT YES)
        endif ()
    endif ()

    if (NOT CMAKE_OSX_ARCHITECTURES)
        if (C74_BUILD_FAT)
            set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "macOS architecture" FORCE)
        else ()
            set(CMAKE_OSX_ARCHITECTURES ${CMAKE_SYSTEM_PROCESSOR} CACHE STRING "macOS architecture" FORCE)
        endif ()
        message("CMAKE_OSX_ARCHITECTURES set to ${CMAKE_OSX_ARCHITECTURES}")
    endif ()
endif ()

# Min-API CMake helpers (SUBDIRLIST, package layout, pre/post target scripts).
#
# max-package.cmake only reads the version (into PACKAGE_VERSION) when a
# package-info.json.in *template* is present; this package ships a static
# package-info.json, so that block is skipped and PACKAGE_VERSION is left empty.
# On Windows that trips the verinfo step's string(REPLACE "." "," PACKAGE_VERSION
# ${PACKAGE_VERSION}) — "requires at least four arguments" — while macOS skips
# the WIN32-only verinfo entirely. Provide the version (matching
# package-info.json) so the Windows build configures.
set(PACKAGE_VERSION "0.0.1")
include(${CMAKE_CURRENT_SOURCE_DIR}/source/min-api/script/min-package.cmake)

# AmbiTap: the shared ambisonics library, pinned as the submodules/AmbiTap
# submodule. Override with -DAmbiTap_ROOT=/path/to/AmbiTap to build against a
# sibling checkout during development. Header-only for the math/dsp layers used
# by most objects (binaural additionally needs AmbiTap::fft).
set(AmbiTap_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/submodules/AmbiTap"
    CACHE PATH "Path to the AmbiTap library source tree")
if (NOT EXISTS "${AmbiTap_ROOT}/include/ambitap/ambitap.h")
    message(FATAL_ERROR "AmbiTap not found at ${AmbiTap_ROOT}. Set -DAmbiTap_ROOT.")
endif ()

# Build AmbiTap's targets so objects can pull in what they need:
#  - Eigen3::Eigen   (rotate / decode / binaural matrix math)
#  - AmbiTap::fft    (binaural convolution)
#  - ambitap         (interface target; with SOFA on it carries libmysofa and
#                     defines AMBITAP_HAS_SOFA for the binaural~ `sofa` attr)
# Header-only objects (encode) just use ${AmbiTap_ROOT}/include directly.
# AmbiTap resolves Eigen via find_package then a pinned FetchContent download;
# tests are off here because it is not the top-level project.
# SOFA HRTF loading pulls in libmysofa. It doesn't build on Windows/MSVC out of
# the box (needs zlib, and its export macros trip C2491), so scope SOFA to
# non-Windows for now; binaural~ still builds everywhere with the built-in KEMAR
# HRTF — only the `sofa` attribute is absent on Windows. (Windows SOFA is a
# tracked follow-up, alongside the Pd binaural~ SOFA work.)
if (WIN32)
    set(AMBITAP_ENABLE_SOFA OFF CACHE BOOL "SOFA HRTF loading for ambitap.binaural~")
else ()
    set(AMBITAP_ENABLE_SOFA ON CACHE BOOL "SOFA HRTF loading for ambitap.binaural~")
endif ()
add_subdirectory(${AmbiTap_ROOT} ${CMAKE_CURRENT_BINARY_DIR}/ambitap EXCLUDE_FROM_ALL)

# Min-Lib, if present (header/static helpers shared across objects).
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/source/min-lib/CMakeLists.txt")
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source/min-lib)
endif ()

# ---- AmbiTap UI widget bundles (v8ui) ---------------------------------------
# The widget sources live in the AmbiTap library repo (ui/ — one shared model,
# an mgraphics renderer backend; see its ui/UI.md). This package BUILDS the
# bundles rather than checking in generated code: npm ci + esbuild in
# ${AmbiTap_ROOT}/ui, staged into javascript/ (gitignored) where Max resolves
# [v8ui @jsfile ambitap.<widget>.js]. Requires node/npm on PATH; skip with
# -DAMBITAP_MAX_BUILD_UI=OFF.
option(AMBITAP_MAX_BUILD_UI "Build the AmbiTap v8ui widget bundles into javascript/" ON)
if (AMBITAP_MAX_BUILD_UI)
    find_program(NPM_EXECUTABLE NAMES npm npm.cmd)
    if (NOT NPM_EXECUTABLE)
        message(FATAL_ERROR "npm not found — required for the v8ui widget bundles (javascript/). "
                            "Install node, or configure with -DAMBITAP_MAX_BUILD_UI=OFF.")
    endif ()
    set(AMBITAP_UI_DIR "${AmbiTap_ROOT}/ui")
    if (NOT EXISTS "${AMBITAP_UI_DIR}/package.json")
        message(FATAL_ERROR "AmbiTap ui/ not found at ${AMBITAP_UI_DIR} — update the submodule "
                            "or point -DAmbiTap_ROOT at a checkout that has the UI layer.")
    endif ()

    # Re-run only when the widget sources change (npm ci is not cheap).
    file(GLOB_RECURSE AMBITAP_UI_SOURCES CONFIGURE_DEPENDS
        "${AMBITAP_UI_DIR}/core/*.ts"
        "${AMBITAP_UI_DIR}/render/*.ts"
        "${AMBITAP_UI_DIR}/widgets/*.ts"
        "${AMBITAP_UI_DIR}/hosts/*.ts")
    set(AMBITAP_UI_STAMP "${CMAKE_CURRENT_BINARY_DIR}/ambitap_ui.stamp")
    add_custom_command(
        OUTPUT "${AMBITAP_UI_STAMP}"
        COMMAND "${NPM_EXECUTABLE}" ci --no-audit --no-fund
        COMMAND "${NPM_EXECUTABLE}" run build
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}/javascript"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${AMBITAP_UI_DIR}/dist/max"
                "${CMAKE_CURRENT_SOURCE_DIR}/javascript"
        COMMAND ${CMAKE_COMMAND} -E touch "${AMBITAP_UI_STAMP}"
        DEPENDS ${AMBITAP_UI_SOURCES}
                "${AMBITAP_UI_DIR}/package.json"
                "${AMBITAP_UI_DIR}/package-lock.json"
                "${AMBITAP_UI_DIR}/scripts/build.mjs"
        WORKING_DIRECTORY "${AMBITAP_UI_DIR}"
        COMMENT "AmbiTap v8ui widget bundles: ${AMBITAP_UI_DIR} -> javascript/"
        VERBATIM)
    add_custom_target(ambitap_ui ALL DEPENDS "${AMBITAP_UI_STAMP}")
endif ()

# One external per folder under source/projects/.
SUBDIRLIST(PROJECT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/source/projects)
foreach (project_dir ${PROJECT_DIRS})
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/source/projects/${project_dir}/CMakeLists.txt")
        message("Generating: ${project_dir}")
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source/projects/${project_dir})
    endif ()
endforeach ()
