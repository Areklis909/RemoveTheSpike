cmake_minimum_required(VERSION 3.9)

set(TARGET CommandLineParser)

include_directories(${CMAKE_SOURCE_DIR}/include)

project(${TARGET} VERSION 1.0.1 DESCRIPTION "CommandLineParser")

add_library(${TARGET} CommandLineParser.cpp)

target_link_libraries(${TARGET} 
    boost_program_options)