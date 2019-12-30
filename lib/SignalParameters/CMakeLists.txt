cmake_minimum_required(VERSION 3.9)

set(TARGET SignalParameters)
set(LIB_LIST     LevinsonDurbin
    ModelStability
    armadillo)

include_directories(${CMAKE_SOURCE_DIR}/include)

project(${TARGET} VERSION 1.0.1 DESCRIPTION "SignalParameters")

add_library(${TARGET} SignalParameters.cpp)

target_link_libraries(${TARGET} ${LIB_LIST})