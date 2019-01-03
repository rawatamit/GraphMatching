#ifndef VERTEXBOOKKEEPING_H
#define VERTEXBOOKKEEPING_H

#include "PreferenceList.h"

// keep track of preference list indices for a vertex
// and bookkeeping for when the proposal index (begin) should be updated
struct VertexBookkeeping {
    // [begin, end)
    // begin is also the proposal index
    PreferenceList::SizeType begin;
    PreferenceList::SizeType end;

    // current level of the vertex
    int level;

    // is the vertex in the free list
    bool in_free_list;

public:
    VertexBookkeeping()
            : VertexBookkeeping(0, 0)
    {}

    VertexBookkeeping(PreferenceList::SizeType begin, PreferenceList::SizeType end)
            : begin(begin), end(end), level(0), in_free_list(false)
    {}

    bool is_exhausted() {
        return begin >= end;
    }
};

#endif // VERTEXBOOKKEEPING_H
