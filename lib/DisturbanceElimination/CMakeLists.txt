cmake_minimum_required(VERSION 3.9)

set(TARGET DisturbanceElimination)

include_directories(${CMAKE_SOURCE_DIR}/include)

project(${TARGET} VERSION 1.0.1 DESCRIPTION "DisturbanceElimination")

add_library(${TARGET} SHARED DisturbanceElimination.cpp)

target_link_libraries(${TARGET} 
    FileHandler
    SignalParameters
    ConfigProcessor
    VariadicKalmanFilter)