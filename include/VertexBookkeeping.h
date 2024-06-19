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
    PreferenceList::SizeType tied_index;

    // [begin, end)
    // begin_lq is proposal index for PrefLQ
    PreferenceList::SizeType begin_lq;
    PreferenceList::SizeType end_lq;

    // current level of the vertex
    int level;

    // is the vertex in the free list
    bool in_free_list;

    // Residual capacity of this vertex
    int residual;

    // map to store the marked vertices
    std::map<VertexPtr, bool> marked;

    // boolean which denotes the star status of a level
    bool star;

public:
    VertexBookkeeping()
            : VertexBookkeeping(0, 0, 0, 0)
    {}

    VertexBookkeeping(PreferenceList::SizeType begin, PreferenceList::SizeType end, PreferenceList::SizeType tied_index,
                      int residual)
            : VertexBookkeeping(begin, end, tied_index, 0, 0, residual)
    {}

    VertexBookkeeping(PreferenceList::SizeType begin, PreferenceList::SizeType end, int residual)
            : VertexBookkeeping(begin, end, 0, 0, 0, residual)
    {}
    
    VertexBookkeeping(PreferenceList::SizeType begin,
                  PreferenceList::SizeType end,
                  PreferenceList::SizeType tied_index,
                  PreferenceList::SizeType begin_lq,
                  PreferenceList::SizeType end_lq,
                  int residual = 0)
    : begin(begin), end(end), tied_index(tied_index),
      begin_lq(begin_lq), end_lq(end_lq),
      level(0), in_free_list(false),
      residual(residual),
      star(false)
{}

    bool is_exhausted() const {
        return begin >= end;
    }

    bool is_exhausted_lq() const {
        return begin_lq >= end_lq;
    }
};

#endif // VERTEXBOOKKEEPING_H