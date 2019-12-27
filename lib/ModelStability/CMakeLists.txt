cmake_minimum_required(VERSION 3.9)

set(TARGET ModelStability)

include_directories(${CMAKE_SOURCE_DIR}/include)

project(${TARGET} VERSION 1.0.1 DESCRIPTION "ModelStability")

add_library(${TARGET} SHARED ModelStability.cpp)

target_link_libraries(${TARGET} 
    armadillo)