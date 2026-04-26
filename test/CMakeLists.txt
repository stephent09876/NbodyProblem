#################################################################################################
# Instructions for running the unit tests for now this is the initial unit tests of the integrators:
#  Inside the test directory make a build directory:
#       > mkdir build
#       > cd build
# Compile the unit tests:
#       > cmake ../ ; make
# Run tests:
#       > ./test_integrator
#################################################################################################
add_compile_options(--coverage -O0 -g)
add_link_options(--coverage)

cmake_minimum_required(VERSION 3.28)
project(unittestproject)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
find_package(GTest REQUIRED)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)

FetchContent_Declare(Eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG 5.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML Eigen)

set(SOURCES
    ../src/Particle.cpp
    ../src/Integrator.cpp
    ../src/Gravity.cpp
    test_integrator.cpp)

add_executable(test_integrator ${SOURCES})

target_link_libraries(test_integrator
    PRIVATE
    GTest::gtest
    GTest::gtest_main
    sfml-graphics
    Eigen3::Eigen
)

target_include_directories(test_integrator PRIVATE ../include)

enable_testing()
add_test(NAME IntegratorTests COMMAND test_integrator)

message(STATUS "CMAKE CURRENT SOURCE DIR: ${CMAKE_CURRENT_SOURCE_DIR}")