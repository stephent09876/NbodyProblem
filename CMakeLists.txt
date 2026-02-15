#########################################################
# This cmake file is meant to compile the sfml 3.0 code
#########################################################
cmake_minimum_required(VERSION 3.28)
project(nBodyProject)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# add all source and include files
set(SOURCES
    src/sfml_3_0_demo.cpp
    src/Particle2.cpp)

add_executable(nBodyProject ${SOURCES})
target_include_directories(nBodyProject PRIVATE include)
target_compile_features(nBodyProject PRIVATE cxx_std_17)
target_link_libraries(nBodyProject PRIVATE SFML::Graphics)



set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)