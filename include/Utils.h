#ifndef UTILS_H
#define UTILS_H

#include "TDefs.h"

// a new id is of the form id^k
IdType get_vertex_id(const IdType& id, int k);

// get the original vertex id given an id
IdType get_original_id(const IdType& id);

// a dummy id is of the form d^k_id
IdType get_dummy_id(const IdType& id, int k);

// get the id of the vertex from the given dummy id
IdType get_vertex_id_from_dummy(const IdType& id);

// return the dummy level from the given id
int get_dummy_level(const IdType& id);

#endif
