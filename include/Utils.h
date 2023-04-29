#ifndef UTILS_H
#define UTILS_H

#include "TDefs.h"
#include "GraphReader.h"
#include "BipartiteGraph.h"
#include "PreferenceList.h"
#include "MatchingAlgorithm.h"

// convert token to string representation
const char* token_to_string(Token tok);

// convert string s to an integer in base 10
int to_integer(const std::string& s);

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
RankType compute_rank(VertexPtr u, const PreferenceList& pref_list);

// print matching in a compatible format with other code as described here
// a,b,index(pref_list^a(b))
void print_matching(std::shared_ptr<BipartiteGraph> G,
                    const Matching& M, std::ostream& out);

#endif
