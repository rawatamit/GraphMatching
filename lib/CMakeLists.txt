cmake_minimum_required(VERSION 3.0)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(graph
                MatchingAlgorithm.cc StableMarriage.cc Popular.cc
                SReduction.cc RHeuristicHRLQ.cc HHeuristicHRLQ.cc
                YokoiEnvyfreeHRLQ.cc MaximalEnvyfreeHRLQ.cc
                BipartiteGraph.cc PartnerList.cc PreferenceList.cc Vertex.cc
                GraphReader.cc Utils.cc)
