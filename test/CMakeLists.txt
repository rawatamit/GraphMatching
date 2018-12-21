cmake_minimum_required(VERSION 3.0)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(testrunner TestPreferenceList.cc TestRunner.cc)

target_link_libraries(testrunner graph)
