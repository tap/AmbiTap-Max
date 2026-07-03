# ambitap.xtc~ — stereo/binaural → two loudspeakers with crosstalk cancellation.

cmake_minimum_required(VERSION 3.19)

set(C74_MIN_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../min-api)
include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

include_directories(
    "${C74_INCLUDES}"
)

set(SOURCE_FILES
    ${PROJECT_NAME}.cpp
)

add_library(
    ${PROJECT_NAME}
    MODULE
    ${SOURCE_FILES}
)

# AmbiTap headers + AmbiTap::fft (the vendored Ooura rdft used by both the
# dsp::xtc design pass and the partitioned convolvers). No Eigen and no SOFA:
# the plant is reconstructed from the built-in SH-domain KEMAR tables and the
# 2x2 inverse is closed form.
target_include_directories(${PROJECT_NAME} PRIVATE "${AmbiTap_ROOT}/include")
target_link_libraries(${PROJECT_NAME} PRIVATE AmbiTap::fft)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

# min-posttarget pins CXX_STANDARD to 17; AmbiTap requires C++20 (std::bit_cast).
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}_test.cpp")
    include(${C74_MIN_API_DIR}/test/min-object-unittest.cmake)
endif ()
