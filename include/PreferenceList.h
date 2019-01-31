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
    typedef ContainerType::const_iterator ConstIterator;

private:
    RankType cur_rank_;        // the current unused rank
    ContainerType pref_list_;  // strictly ordered preference list

public:
    PreferenceList();
    virtual ~PreferenceList() = default;

    Iterator begin();
    Iterator end();

    ConstIterator cbegin() const;
    ConstIterator cend() const;

    /// size of the preference list
    SizeType size() const;

    /// is the preference list empty
    bool empty();

    /// insert element at end
    void emplace_back(VertexPtr v);

    /// find the vertex in the container
    ConstIterator find(VertexPtr v) const;

    /// find the index of the given vertex
    SizeType find_index(VertexPtr v) const;
    
    /// get pref list element at given index
    PrefListElement at(SizeType index) const;

    friend std::ostream& operator<<(std::ostream& out, PreferenceList& pl);
    friend std::ostream& operator<<(std::ostream& out, PreferenceList* pl);
};

#endif
