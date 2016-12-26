#ifndef UTILS_H
#define UTILS_H

#include "TDefs.h"

// a new id is of the form id^k
IdType get_vertex_id(const IdType& id, int k);

// a dummy id is of the form d^k_id
IdType get_dummy_id(const IdType& id, int k);

// return the dummy level from the given id
int get_dummy_level(const IdType& id);

#endif
