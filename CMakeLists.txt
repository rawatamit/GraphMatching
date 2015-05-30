cmake_minimum_required(VERSION 2.8)
project(graphmatching)

# compiler flags
add_definitions("-std=c++11 -Wall")

# include directory and the libraries we will build
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_subdirectory(lib)

# build and install
add_executable(graphmatching main.cc)
install(TARGETS graphmatching RUNTIME DESTINATION bin)
