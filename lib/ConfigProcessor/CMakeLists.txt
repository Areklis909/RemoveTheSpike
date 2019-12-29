cmake_minimum_required(VERSION 3.9)

#include_directories(${CMAKE_SOURCE_DIR}/include/ConfigProcessor)

set(TARGET ConfigProcessor)

project(${TARGET} VERSION 1.0.1 DESCRIPTION "ConfigProcessor")

add_library(${TARGET} ConfigProcessor.cpp)

target_link_libraries(${TARGET} config)

#install(TARGETS ${TARGET}
#    LIBRARY DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/lib
#    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#install(FILES "lib${TARGET}" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/lib)