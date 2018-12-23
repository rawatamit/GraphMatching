#ifndef UTILS_H
#define UTILS_H

#include "TDefs.h"
#include "BipartiteGraph.h"
#include "PreferenceList.h"
#include "MatchingAlgorithm.h"

// convert string s to an integer in base 10
int to_integer(const std::string& s);

// get partners for v in M
PartnerList get_partners(std::shared_ptr<MatchingAlgorithm::MatchedPairListType> M, VertexPtr v);

// # of partners for v in M
PartnerList::SizeType number_of_partners(std::shared_ptr<MatchingAlgorithm::MatchedPairListType> M, VertexPtr v);

// does v have at least one partner in M
bool has_partner(std::shared_ptr<MatchingAlgorithm::MatchedPairListType> M, VertexPtr v);

// get matched partner for v in M
VertexPtr get_partner(std::shared_ptr<MatchingAlgorithm::MatchedPairListType> M, VertexPtr v);

// get matched partner from partner list
VertexPtr get_partner(const PartnerList& partner_list);

// size of matching M
int matching_size(std::shared_ptr<MatchingAlgorithm::MatchedPairListType> M);

// get vertex by id in graph G
VertexPtr get_vertex_by_id(std::shared_ptr<BipartiteGraph> G, const IdType& id);

// read graph from a given filepath
std::shared_ptr<BipartiteGraph> read_graph(const std::string& filepath);

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
void print_matching(std::shared_ptr<BipartiteGraph> G,
                    std::shared_ptr<MatchingAlgorithm::MatchedPairListType> M, std::ostream& out);

#endif
