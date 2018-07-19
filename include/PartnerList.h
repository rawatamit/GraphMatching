#ifndef PARTNER_LIST_H
#define PARTNER_LIST_H

#include "TDefs.h"
#include <set>

struct Partner {
    VertexPtr partner;
    RankType rank;
    int level;

    Partner(VertexPtr partner, RankType rank, int level)
        : partner(partner), rank(rank), level(level)
    {}
    
    // the priority of this vertex is defined by its level and rank
    // this vertex is more preferred than every vertex with a lower level
    // and more preferred than vertices with same level but a higher rank
    // the worst partners are stored at the 'beginning' of the set
    bool operator< (const Partner& b) const {
        if (level < b.level) {
            return true;
        } else if (level > b.level) {
            return false;
        } else if (rank > b.rank) {
            return true;
        } else if (rank < b.rank) {
            return false;
        } else { // should never happen
            return true;
        }
    }
};

class PartnerList {
public:
    typedef typename std::set<Partner> ContainerType;
    typedef ContainerType::iterator Iterator;
    typedef ContainerType::const_iterator ConstIterator;
    typedef ContainerType::size_type SizeType;

private:
    ContainerType partners_;

public:
    PartnerList();
    virtual ~PartnerList();

    bool empty() const;
    SizeType size() const;
    ConstIterator cbegin() const;
    ConstIterator cend() const;
    Iterator begin();
    Iterator end();

    /// is this vertex in the list
    ConstIterator find(VertexPtr v) const;

    /// add a vertex to the list of matched partners
    void add_partner(VertexPtr partner, RankType rank, int level);

    /// return the worst partner matched to this vertex
    Partner get_least_preferred();

    /// remove partner from list
    void remove(VertexPtr v);

/*
    std::ostream& operator<<(std::ostream& out, PartnerList& pl);
    std::ostream& operator<<(std::ostream& out, PartnerList* pl);
*/
};

#endif
