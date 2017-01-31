#include "PartnerList.h"
#include "Vertex.h"
#include <algorithm>
#include <sstream>

PartnerList::PartnerList()
{}

PartnerList::~PartnerList() {
    partners_.clear();
}

bool PartnerList::empty() const {
    return size() == 0;
}

PartnerList::SizeType PartnerList::size() const {
    return partners_.size();
}

PartnerList::ConstIterator PartnerList::cbegin() const {
    return partners_.cbegin();
}

PartnerList::ConstIterator PartnerList::cend() const {
    return partners_.cend();
}

PartnerList::Iterator PartnerList::begin() {
    return partners_.begin();
}

PartnerList::Iterator PartnerList::end() {
    return partners_.end();
}

PartnerList::ConstIterator PartnerList::find(VertexPtr v) const {
    for (auto i = cbegin(), e = cend(); i != e; ++i) {
        if (get_vertex(i) == v) {
            return i;
        }
    }

    return cend();
}

/// add a vertex to the list of matched partners
void PartnerList::add_partner(const PartnerType& partner) {
    // partners_.add(std::make_unique<Heap::EntryType>(partner, partner->first));
    partners_.emplace_back(partner);
    std::push_heap(partners_.begin(), partners_.end());
}

/// return details for the worst partner matched to this vertex
PartnerList::Iterator PartnerList::get_least_preferred() {
    return partners_.begin();
}

void PartnerList::remove(VertexPtr v) {
    SizeType vindex = -1;

    // if only a 1 sized heap, just return
    if (size() == 1) { partners_.clear(); return; }

    for (SizeType i = 0; i < size(); ++i) {
        if (get_vertex(begin() + i) == v) {
            vindex = i;
            break;
        }
    }

    if (vindex != -1) {
        // auto& u = partners_.at(vindex);
        // auto& v = partners_.at(size()-1);
        // std::swap(partners_.begin()+vindex, partners_.end()-1);
        // partners_.pop_back();
    }
}

/// remove the least preferred among the current partners
void PartnerList::remove_least_preferred() {
    std::pop_heap(partners_.begin(), partners_.end());
    partners_.pop_back();
}

RankType PartnerList::get_rank(const PartnerList::ConstIterator& it) const {
    return it->first;
}

VertexPtr PartnerList::get_vertex(const PartnerList::ConstIterator& it) const {
    return it->second;
}

RankType PartnerList::get_rank(const PartnerList::Iterator& it) const {
    return it->first;
}

VertexPtr PartnerList::get_vertex(const PartnerList::Iterator& it) const {
    return it->second;
}

std::ostream& operator<<(std::ostream& out, PartnerList& pl) {
    return out << &pl;
}

std::ostream& operator<<(std::ostream& out, PartnerList* pl) {
    std::stringstream stmp;

    for (auto i = pl->partners_.begin(), e = pl->partners_.end();
         i != e; ++i)
    {
        stmp << pl->get_vertex(i)->get_id();

        if (i+1 == e) {
           stmp << ';';
        } else {
            stmp << ", ";
        }
    }

    return out << stmp.str();
}
