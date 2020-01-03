cmake_minimum_required(VERSION 3.9)

set(TARGET Signal)

include_directories(${CMAKE_SOURCE_DIR}/include)

project(${TARGET} VERSION 1.0.1 DESCRIPTION "Signal")

add_library(${TARGET} Signal.cpp)

target_link_libraries(${TARGET} 
    )