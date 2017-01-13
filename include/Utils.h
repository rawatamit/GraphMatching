#ifndef UTILS_H
#define UTILS_H

#include "TDefs.h"
#include "MatchingAlgorithm.h"

// a new id is of the form id^k
IdType get_vertex_id(const IdType& id, int k);

// a dummy id is of the form d^k_id
IdType get_dummy_id(const IdType& id, int k);

// return the dummy level from the given id
int get_dummy_level(const IdType& id);

// print matching in a compatible format with other code as described here
// a,b,index(pref_list^a(b))
void print_matching_standard_format(MatchingAlgorithm::MatchedPairListType& matched_pairs,
                                    std::ostream& out);

#endif
