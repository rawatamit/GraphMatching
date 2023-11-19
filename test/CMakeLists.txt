cmake_minimum_required(VERSION 3.11)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

include(FetchContent)

FetchContent_Declare(Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.3.2)

# make Catch2 available
FetchContent_MakeAvailable(Catch2)

# add Catch2 include directory to path
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)

add_executable(testrunner
               TestDefs.cc TestRunner.cc
               TestPreferenceList.cc TestPartnerList.cc TestUtils.cc
               TestStableMarriage.cc TestPopular.cc TestHR.cc TestYokoi.cc TestMaximalEnvyfreeHRLQ.cc TestParser.cc TestWeakStable.cc TestFavouriteNeighbour.cc TestPrefers.cc)

target_link_libraries(testrunner PRIVATE graph Catch2::Catch2WithMain)
