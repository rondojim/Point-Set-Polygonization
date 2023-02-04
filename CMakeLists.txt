# Created by the script cgal_create_CMakeLists
# This is the CMake script for compiling a set of CGAL applications.

cmake_minimum_required(VERSION 3.1...3.23)

project(to_polygon)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# CGAL and its components
find_package(CGAL QUIET COMPONENTS)
set(CMAKE_BUILD_TYPE "Release")

if(NOT CGAL_FOUND)
  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()
endif()

# Boost and its components
find_package(Boost REQUIRED)

if(NOT Boost_FOUND)
  message(STATUS "This project requires the Boost library, and will not be compiled.")

  return()
endif()

# include for local directory

# include for local package
include_directories(BEFORE ../include)

# Creating entries for target: to_polygon
# ############################
add_executable(to_polygon src/ch2polyline_algo.cpp src/graham_scan.cpp src/incremental_algo.cpp src/main.cpp src/onion_algo.cpp src/pick_algo.cpp src/utils.cpp)

add_to_cached_list(CGAL_EXECUTABLE_TARGETS to_polygon)

# Link the executable to CGAL and third-party libraries
target_link_libraries(to_polygon PRIVATE CGAL::CGAL)
