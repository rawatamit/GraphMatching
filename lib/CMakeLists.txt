cmake_minimum_required(VERSION 3.0)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(graph
                MatchingAlgorithm.cc NProposingMatching.cc
                YokoiEnvyfreeHRLQ.cc MaximalEnvyfreeHRLQ.cc
                HHeuristicHRLQ.cc RHeuristicHRLQ.cc
                BipartiteGraph.cc Partner.cc PartnerList.cc PreferenceList.cc Vertex.cc
                GraphReader.cc Utils.cc)
