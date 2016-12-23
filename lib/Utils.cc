#include "Utils.h"
#include <sstream>

// a new id is of the form id^k
IdType get_vertex_id(const IdType& id, int k) {
    std::stringstream stmp;
    stmp << id << '^' << k;
    return stmp.str();
}

// get the original vertex id given an id
IdType get_original_id(const IdType& id) {
    return id.substr(0, id.find('^'));
}

// a dummy id is of the form d^k_id
IdType get_dummy_id(const IdType& id, int k) {
    std::stringstream stmp;
    stmp << "d^" << k << '_' << id;
    return stmp.str();
}

// get the id of the vertex from the given dummy id
IdType get_vertex_id_from_dummy(const IdType& id) {
    return id.substr(id.find('_')+1);
}

// return the dummy level from the given id
int get_dummy_level(const IdType& id) {
    auto caret_pos = id.find('^');
    auto underscore_pos = id.find('_');
    return std::atoi(id.substr(caret_pos+1, underscore_pos-caret_pos-1).c_str());
}
