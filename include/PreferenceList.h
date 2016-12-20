#ifndef PREFERENCE_LIST_H
#define PREFERENCE_LIST_H

#include <limits>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

class Vertex;  // forward declaration for Vertex

class PreferenceList {
public:
    // some useful typedefs
    typedef int RankType;
    typedef std::shared_ptr<Vertex> VertexType;
    typedef typename std::pair<RankType, VertexType> ElementType;
    typedef typename std::vector<ElementType> ContainerType;
    typedef ContainerType::size_type SizeType;
    typedef ContainerType::iterator Iterator;

private:
    static const RankType RANK_INFINITY = std::numeric_limits<RankType>::max();
    SizeType start_iter_;
    SizeType end_iter_;
    RankType cur_rank_;        // the current unused rank
    ContainerType pref_list_;  // strict ordered preference list

    /// find the vertex in the container
    SizeType find(const VertexType& v);

public:
    PreferenceList();
    PreferenceList(const PreferenceList& that);
    PreferenceList& operator=(const PreferenceList& that);
    virtual ~PreferenceList();

    SizeType begin();
    SizeType end();

    /// clear the list
    void clear();

    /// size of the preference list
    SizeType size();

    /// is the preference list empty
    bool empty();

    /// insert element at end
    void emplace_back(const VertexType& v);

    /// functions to return the vertex and rank from an ElementType
    VertexType get_vertex(const ElementType& it);
    RankType get_rank(const ElementType& it);

    /// functions to return the vertex and rank from a given iterator
    VertexType get_vertex(SizeType index);
    RankType get_rank(SizeType index);

    /// return the vertex this vertex will now propose to
    VertexType get_proposal_partner();

    /// does this vertex prefer a to b
    bool is_ranked_better(const VertexType& a, const VertexType& b);

    /// restrict the preference list [begin(), pref_list_[v]]
    void restrict_preferences(const VertexType& v);

    /// remove the first element from the preference list
    void remove_first();

    friend std::ostream& operator<<(std::ostream& out, PreferenceList& pl);
    friend std::ostream& operator<<(std::ostream& out, PreferenceList* pl);
};

#endif
