cmake_minimum_required(VERSION 3.0)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(testrunner TestRunner.cc TestPreferenceList.cc TestPartnerList.cc TestUtils.cc TestStableMarriage.cc)

target_link_libraries(testrunner graph)
