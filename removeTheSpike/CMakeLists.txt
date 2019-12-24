project(RemoveTheSpike)

set(TARGET RemoveTheSpike)

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

set(SOURCES src/Source.cpp
    src/SignalParameters.cpp
    src/DataSaver.cpp
    src/FileHandler.cpp
    src/LevinsonDurbin.cpp
    src/VariadicKalmanFilter.cpp
    src/ModelStability.cpp
    src/DisturbanceElimination.cpp
    src/ConfigProcessor.cpp
)

add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET} PRIVATE armadillo)
target_link_libraries(${TARGET} PRIVATE sndfile)
target_link_libraries(${TARGET} PRIVATE config)
