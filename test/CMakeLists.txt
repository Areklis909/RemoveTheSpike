set(TARGET RemoveTheSpikeTest)

cmake_minimum_required(VERSION 2.6)
 
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

find_library(libconfig_location NAMES libconfig++.so)
message(STATUS ${libconfig_location})
add_library(config SHARED IMPORTED)
set_target_properties(config PROPERTIES IMPORTED_LOCATION ${libconfig_location})

set(SOURCES main.cpp
    ../removeTheSpike/src/ConfigProcessor.cpp)
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests ${SOURCES})
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread config)

install(FILES TestConfig.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})