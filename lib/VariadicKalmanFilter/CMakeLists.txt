cmake_minimum_required(VERSION 3.9)

set(TARGET VariadicKalmanFilter)

include_directories(${CMAKE_SOURCE_DIR}/include)

project(${TARGET} VERSION 1.0.1 DESCRIPTION "VariadicKalmanFilter")

add_library(${TARGET} SHARED VariadicKalmanFilter.cpp)

target_link_libraries(${TARGET} 
    armadillo
    )