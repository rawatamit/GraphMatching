#ifndef UTILS_H
#define UTILS_H

#include "TDefs.h"
#include "BipartiteGraph.h"

// convert string s to an integer in base 10
int to_integer(const std::string& s);

// a new id is of the form id^k
IdType get_vertex_id(const IdType& id, int k);

// return the vertex level from the given id
int get_vertex_level(const IdType& id);

// a dummy id is of the form d^k_id
IdType get_dummy_id(const IdType& id, int k);

// return the dummy level from the given id
int get_dummy_level(const IdType& id);

// print matching in a compatible format with other code as described here
// a,b,index(pref_list^a(b))
void print_matching(const std::unique_ptr<BipartiteGraph>& G,
                    MatchedPairListType& M, std::ostream& out);

#endif
