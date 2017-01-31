#ifndef PREFERENCE_LIST_H
#define PREFERENCE_LIST_H

#include <limits>
#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include "TDefs.h"

class PreferenceList {
public:
    typedef typename std::pair<RankType, VertexPtr> ElementType;
    typedef typename std::vector<ElementType> ContainerType;
    typedef ContainerType::size_type SizeType;
    typedef ContainerType::iterator Iterator;

private:
    static const RankType RANK_INFINITY = std::numeric_limits<RankType>::max();
    SizeType start_iter_;
    SizeType end_iter_;
    RankType cur_rank_;        // the current unused rank
    ContainerType pref_list_;  // strict ordered preference list

public:
    PreferenceList();
    PreferenceList(const PreferenceList& that);
    PreferenceList& operator=(const PreferenceList& that);
    virtual ~PreferenceList();

    SizeType begin();
    SizeType end();

    // these provide the end and beginning
    // of the whole preference list, and not [start_iter_, end_iter_)
    Iterator all_begin();
    Iterator all_end();

    /// clear the list
    void clear();

    /// size of the preference list
    SizeType size();

    /// is the preference list empty
    bool empty();

    /// insert element at end
    void emplace_back(VertexPtr v);

    /// find the vertex in the container
    SizeType find(VertexPtr v);

    /// functions to return the vertex and rank from an ElementType
    VertexPtr get_vertex(const ElementType& it);
    RankType get_rank(const ElementType& it);

    /// functions to return the vertex and rank from a given given
    VertexPtr get_vertex(SizeType index);
    RankType get_rank(SizeType index);

    /// return the index of the vertex that this vertex will now propose to
    SizeType get_proposal_index();

    /// does this vertex prefer a to b
    bool is_ranked_better(VertexPtr a, VertexPtr b);

    /// restrict the preference list [begin(), pref_list_[v]]
    void restrict_preferences(VertexPtr v);

    /// remove the first element from the preference list
    void remove_first();

    friend std::ostream& operator<<(std::ostream& out, PreferenceList& pl);
    friend std::ostream& operator<<(std::ostream& out, PreferenceList* pl);
};

#endif
