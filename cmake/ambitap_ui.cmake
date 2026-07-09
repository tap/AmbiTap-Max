# Build the AmbiTap v8ui widget bundles: npm ci + esbuild in UI_DIR, staged
# into DEST, touching STAMP on success. Invoked as one command from the
# ambitap_ui custom target:
#
#   cmake -DNPM=... -DUI_DIR=... -DDEST=... -DSTAMP=... -P ambitap_ui.cmake
#
# One cmake -P script instead of a COMMAND sequence on purpose: on Windows,
# MSBuild renders multiple COMMANDs as one batch script, and invoking
# npm.cmd from a batch script TRANSFERS control (batch chaining) — every
# command after the first npm call silently never runs.
foreach (var NPM UI_DIR DEST STAMP)
    if (NOT DEFINED ${var})
        message(FATAL_ERROR "ambitap_ui.cmake: ${var} not set")
    endif ()
endforeach ()

execute_process(
    COMMAND "${NPM}" ci --no-audit --no-fund
    WORKING_DIRECTORY "${UI_DIR}"
    RESULT_VARIABLE rc)
if (NOT rc EQUAL 0)
    message(FATAL_ERROR "ambitap_ui: npm ci failed in ${UI_DIR}")
endif ()

execute_process(
    COMMAND "${NPM}" run build
    WORKING_DIRECTORY "${UI_DIR}"
    RESULT_VARIABLE rc)
if (NOT rc EQUAL 0)
    message(FATAL_ERROR "ambitap_ui: npm run build failed in ${UI_DIR}")
endif ()

file(GLOB bundles "${UI_DIR}/dist/max/*.js")
if (NOT bundles)
    message(FATAL_ERROR "ambitap_ui: no bundles in ${UI_DIR}/dist/max")
endif ()
file(MAKE_DIRECTORY "${DEST}")
file(COPY ${bundles} DESTINATION "${DEST}")
file(TOUCH "${STAMP}")
