set(TARGET RemoveTheSpikeTest)

cmake_minimum_required(VERSION 2.6)
 
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(SOURCES main.cpp
    ../lib/ConfigProcessor/ConfigProcessor.cpp)
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests ${SOURCES})
target_link_libraries(runTests 
    ${GTEST_LIBRARIES}
    pthread
    config
    ConfigProcessor)

install(FILES TestConfig.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})