cmake_minimum_required(VERSION 3.0)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(testrunner
               TestDefs.cc TestRunner.cc
               TestPreferenceList.cc TestPartnerList.cc TestUtils.cc
               TestStableMarriage.cc TestPopular.cc TestHR.cc TestYokoi.cc TestMaximalEnvyfreeHRLQ.cc TestParser.cc)

target_link_libraries(testrunner graph)
