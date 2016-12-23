#include "PartnerList.h"
#include "Vertex.h"
#include <algorithm>

PartnerList::PartnerList()
{}

PartnerList::~PartnerList() {
    partners_.clear();
}

PartnerList::SizeType PartnerList::size() const {
    return partners_.size();
}

PartnerList::Iterator PartnerList::begin() {
    return partners_.begin();
}

PartnerList::Iterator PartnerList::end() {
    return partners_.end();
}

/// add a vertex to the list of matched partners
void PartnerList::add_partner(const PartnerType& partner) {
    partners_.emplace_back(partner);
    std::push_heap(partners_.begin(), partners_.end());
}

/// return details for the worst partner matched to this vertex
PartnerList::Iterator PartnerList::get_least_preferred() {
    return partners_.begin();
}

/// remove the least preferred among the current partners
void PartnerList::remove_least_preferred() {
    std::pop_heap(partners_.begin(), partners_.end());
    partners_.pop_back();
}

RankType PartnerList::get_rank(const PartnerList::Iterator& it) const {
    return it->first;
}

VertexPtr PartnerList::get_vertex(const PartnerList::Iterator& it) const {
    return it->second;
}
