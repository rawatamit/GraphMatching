#ifndef UTILS_H
#define UTILS_H

#include "TDefs.h"
#include "BipartiteGraph.h"
#include "PreferenceList.h"
#include "MatchingAlgorithm.h"

// convert string s to an integer in base 10
int to_integer(const std::string& s);

// does v have at least one partner in M
bool has_partner(const MatchingAlgorithm::MatchedPairListType& M, VertexPtr v);

// size of matching M
int matching_size(const MatchingAlgorithm::MatchedPairListType& M);

// a new id is of the form id^k
IdType get_vertex_id(const IdType& id, int k);

// return the vertex level from the given id
int get_vertex_level(const IdType& id);

// a dummy id is of the form d^k_id
IdType get_dummy_id(const IdType& id, int k);

// return the dummy level from the given id
int get_dummy_level(const IdType& id);

// compute rank from vertex index and level
RankType compute_rank(VertexPtr u, const PreferenceList& pref_list, int level);

// print matching in a compatible format with other code as described here
// a,b,index(pref_list^a(b))
void print_matching(const std::unique_ptr<BipartiteGraph>& G,
                    MatchingAlgorithm::MatchedPairListType& M, std::ostream& out);

#endif
