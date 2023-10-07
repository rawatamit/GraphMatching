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
    std::unordered_map<RankType, std::vector<PrefListElement>> tie_map_; //map to store when a particular rank has ties 
    std::unordered_map<RankType, bool> is_tied_;

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

    //set the ties at the given rank 
    void set_ties(RankType rank, PreferenceList p); 

    //get the tied elements at the given rank 
    [[nodiscard]] std::vector<PrefListElement> get_ties(RankType rank) const; 

    //find if the particular rank is tied 
    bool isTied(RankType rank) const;

    void printList();

    friend std::ostream& operator<<(std::ostream& out, const PreferenceList& pl);
    friend std::ostream& operator<<(std::ostream& out, const PreferenceList* pl);
};

#endif
