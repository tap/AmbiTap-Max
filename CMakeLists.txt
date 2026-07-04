# AmbiTap-Max — Max/MSP externals over the AmbiTap ambisonics library.
# Structure follows the Cycling '74 Min-DevKit package convention.

cmake_minimum_required(VERSION 3.19)
project(AmbiTap-Max)

enable_testing()

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
set(AMBITAP_ENABLE_SOFA ON CACHE BOOL "SOFA HRTF loading for ambitap.binaural~")
add_subdirectory(${AmbiTap_ROOT} ${CMAKE_CURRENT_BINARY_DIR}/ambitap EXCLUDE_FROM_ALL)

# Min-Lib, if present (header/static helpers shared across objects).
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/source/min-lib/CMakeLists.txt")
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source/min-lib)
endif ()

# One external per folder under source/projects/.
SUBDIRLIST(PROJECT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/source/projects)
foreach (project_dir ${PROJECT_DIRS})
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/source/projects/${project_dir}/CMakeLists.txt")
        message("Generating: ${project_dir}")
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source/projects/${project_dir})
    endif ()
endforeach ()
