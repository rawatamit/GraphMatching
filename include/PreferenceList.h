#ifndef PREFERENCE_LIST_H
#define PREFERENCE_LIST_H

#include <limits>
#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include "TDefs.h"

struct PrefListElement {
    RankType rank;
    VertexPtr vertex;

    PrefListElement(RankType rank, VertexPtr vertex)
        : rank(rank), vertex(vertex)
    {}
};

class PreferenceList {
public:
    typedef typename std::vector<PrefListElement> ContainerType;
    typedef ContainerType::size_type SizeType;
    typedef ContainerType::iterator Iterator;

private:
    static const RankType RANK_INFINITY = std::numeric_limits<RankType>::max();
    RankType cur_rank_;        // the current unused rank
    ContainerType pref_list_;  // strictly ordered preference list

public:
    PreferenceList();
    PreferenceList(const PreferenceList& that);
    virtual ~PreferenceList();

    Iterator begin();
    Iterator end();

    /// size of the preference list
    SizeType size();

    /// is the preference list empty
    bool empty();

    /// insert element at end
    void emplace_back(VertexPtr v);

    /// find the vertex in the container
    Iterator find(VertexPtr v);

    /// find the index of the given vertex
    SizeType find_index(VertexPtr v);
    
    /// get pref list element at given index
    PrefListElement get(SizeType index);

    friend std::ostream& operator<<(std::ostream& out, PreferenceList& pl);
    friend std::ostream& operator<<(std::ostream& out, PreferenceList* pl);
};

#endif
