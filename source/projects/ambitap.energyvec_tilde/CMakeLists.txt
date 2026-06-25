# ambitap.energyvec~ — active-intensity (energy vector) DOA analysis.

cmake_minimum_required(VERSION 3.19)

set(C74_MIN_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../min-api)
include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

include_directories("${C74_INCLUDES}")
set(SOURCE_FILES ${PROJECT_NAME}.cpp)
add_library(${PROJECT_NAME} MODULE ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE "${AmbiTap_ROOT}/include")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}_test.cpp")
    include(${C74_MIN_API_DIR}/test/min-object-unittest.cmake)
endif ()
