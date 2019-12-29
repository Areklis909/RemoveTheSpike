cmake_minimum_required(VERSION 3.9)

set(TARGET LevinsonDurbin)

include_directories(${CMAKE_SOURCE_DIR}/include)

project(${TARGET} VERSION 1.0.1 DESCRIPTION "LevinsonDurbin")

add_library(${TARGET} LevinsonDurbin.cpp)

target_link_libraries(${TARGET} 
    armadillo)