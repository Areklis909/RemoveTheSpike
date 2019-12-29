cmake_minimum_required(VERSION 3.9)

set(TARGET FileHandler)

include_directories(${CMAKE_SOURCE_DIR}/include)

project(${TARGET} VERSION 1.0.1 DESCRIPTION "FileHandler")

add_library(${TARGET} FileHandler.cpp)

target_link_libraries(${TARGET} 
    sndfile)