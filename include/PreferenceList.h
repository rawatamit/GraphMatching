#ifndef PREFERENCE_LIST_H
#define PREFERENCE_LIST_H

#include <limits>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
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
    // The following map is required to store the tied vertices at a particular rank 
    // Whenever a rank is tied, it is added to it with the rank as key and the tied vertices as the value
    std::unordered_map<RankType, std::vector<PrefListElement>> ties_;

public:
    PreferenceList();
    virtual ~PreferenceList() = default;

    Iterator begin();
    Iterator end();

    [[nodiscard]] ConstIterator cbegin() const;
    [[nodiscard]] ConstIterator cend() const;

    /// size of the preference list
    [[nodiscard]] SizeType size() const;

    /// is the preference list empty
    bool empty();

    /// insert element at end
    void emplace_back(VertexPtr v);

    /// find the vertex in the container
    [[nodiscard]] ConstIterator find(VertexPtr v) const;

    /// find the index of the given vertex
    [[nodiscard]] SizeType find_index(VertexPtr v) const;
    
    /// get pref list element at given index
    [[nodiscard]] PrefListElement at(SizeType index) const;

    // set the tie at the given rank 
    void set_tie(RankType rank, VertexPtr v);

    // get the tied elements at the given rank 
    const std::vector<PrefListElement>& get_ties(RankType rank) const;

    // find if the particular rank is tied 
    bool is_tied(RankType rank) const;

    friend std::ostream& operator<<(std::ostream& out, const PreferenceList& pl);
    friend std::ostream& operator<<(std::ostream& out, const PreferenceList* pl);
};

#endif
