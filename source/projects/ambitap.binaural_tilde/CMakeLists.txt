# ambitap.binaural~ — HOA → binaural stereo via SH-HRTF convolution.

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

# AmbiTap headers + Eigen (head-tracking rotation) + tap::dsp (the vendored
# Ooura rdft that the partitioned convolver calls) + the ambitap interface
# target (carries libmysofa and the AMBITAP_HAS_SOFA define when SOFA is on).
target_include_directories(${PROJECT_NAME} PRIVATE "${AmbiTap_ROOT}/include")
target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen tap::dsp ambitap)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

# min-posttarget pins CXX_STANDARD to 17; AmbiTap requires C++20 (std::bit_cast).
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}_test.cpp")
    include(${C74_MIN_API_DIR}/test/min-object-unittest.cmake)
endif ()
