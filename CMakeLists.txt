cmake_minimum_required(VERSION 3.11)
project(graphmatching)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# include directory and the libraries to build
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_subdirectory(lib)
add_subdirectory(test)

add_executable(graphmatching main.cc)
target_link_libraries(graphmatching graph)

install(TARGETS graphmatching RUNTIME DESTINATION bin)
