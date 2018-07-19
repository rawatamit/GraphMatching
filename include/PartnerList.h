#ifndef PARTNER_LIST_H
#define PARTNER_LIST_H

#include "TDefs.h"
// #include "Heap.h"
#include <list>
#include <memory>

struct Partner {
    VertexPtr partner;
    RankType rank;
    int level;

    Partner(VertexPtr partner, RankType rank, int level)
        : partner(partner), rank(rank), level(level)
    {}
};

class PartnerList {
public:
    //typedef typename std::pair<RankType, VertexPtr> PartnerType;
    typedef typename std::list<Partner> ContainerType;
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

Iterator find_least_preferred();
    /// return details for the worst partner matched to this vertex
    Partner get_least_preferred();

    /// remove this partner from the list
    void remove(VertexPtr v);

    /// remove the least preferred among the current partners
    void remove_least_preferred();

/*
    friend std::ostream& operator<<(std::ostream& out, PartnerList& pl);
    friend std::ostream& operator<<(std::ostream& out, PartnerList* pl);
*/
};

#endif
