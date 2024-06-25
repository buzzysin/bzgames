cmake_minimum_required(VERSION 3.22)

project(bzgames)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BZ_BUILD_TESTS "Build tests" ON)

# Where to find custom cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
# Include custom functions
include(${CMAKE_SOURCE_DIR}/cmake/bz/bz_functions.cmake)

if (BZ_BUILD_TESTS)
  # enable_testing()
endif()

# Create an interface target to link to for common compile options
bz_intf(strict
  PARENT  
    args
  COMPILE_ARGS 
    -Wall -Wextra -Werror -Wpedantic
)

# Third party libraries
add_subdirectory(vendor)

# Libraries
add_subdirectory(core)
add_subdirectory(engine)

# Games
add_subdirectory(games)

# Tests
add_subdirectory(tests)