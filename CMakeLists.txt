cmake_minimum_required(VERSION 2.6)
project(graphmatching)

add_executable(graphmatching main.cpp)

install(TARGETS graphmatching RUNTIME DESTINATION bin)
