cmake_minimum_required(VERSION 2.8)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(graph
                #PopularMatching2Sided.cc
                MatchingAlgorithm.cc StableMarriage.cc
                BipartiteGraph.cc PreferenceList.cc Vertex.cc
                GraphReader.cc)
