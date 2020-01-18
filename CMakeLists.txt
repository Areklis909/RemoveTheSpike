cmake_minimum_required(VERSION 3.10)

project(RemoveTheSpike)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(GCC_COVERAGE_COMPILE_FLAGS "-O3 -Wall")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
include_directories(${CMAKE_SOURCE_DIR}/include)

# libarmadillo is installed in /usr/local/lib - find it
find_library(armadillo_location NAMES libarmadillo.so)
message(STATUS ${armadillo_location})
#set armadillo library as IMPORTED
add_library(armadillo SHARED IMPORTED)
set_target_properties(armadillo PROPERTIES IMPORTED_LOCATION ${armadillo_location})

find_library(sndfile_location NAMES libsndfile.so)
message(STATUS ${sndfile_location})
add_library(sndfile SHARED IMPORTED)
set_target_properties(sndfile PROPERTIES IMPORTED_LOCATION ${sndfile_location})

find_library(libconfig_location NAMES libconfig++.so)
message(STATUS ${libconfig_location})
add_library(config SHARED IMPORTED)
set_target_properties(config PROPERTIES IMPORTED_LOCATION ${libconfig_location})

find_library(boost_program_options_location NAMES libboost_program_options.so)
message(STATUS ${boost_program_options_location})
add_library(boost_program_options SHARED IMPORTED)
set_target_properties(boost_program_options PROPERTIES IMPORTED_LOCATION ${boost_program_options_location})

add_subdirectory(lib)
add_subdirectory(test)
add_subdirectory(src)
