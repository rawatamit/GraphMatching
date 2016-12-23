#ifndef PARTNER_LIST_H
#define PARTNER_LIST_H

#include <memory>
#include <vector>
#include "TDefs.h"

class PartnerList {
public:
    typedef typename std::pair<RankType, VertexPtr> PartnerType;
    typedef typename std::vector<PartnerType> ContainerType;
    typedef ContainerType::iterator Iterator;
    typedef ContainerType::size_type SizeType;

private:
    ContainerType partners_;

public:
    PartnerList();
    virtual ~PartnerList();

    SizeType size() const;
    Iterator begin();
    Iterator end();

    /// add a vertex to the list of matched partners
    void add_partner(const PartnerType& partner);

    /// return details for the worst partner matched to this vertex
    Iterator get_least_preferred();

    /// remove the least preferred among the current partners
    void remove_least_preferred();

    RankType get_rank(const Iterator& it) const;
    VertexPtr get_vertex(const Iterator& it) const;
};

#endif
