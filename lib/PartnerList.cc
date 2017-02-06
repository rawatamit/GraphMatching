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
    return partners_.empty();
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
    partners_.emplace_back(partner);
}

/// return details for the worst partner matched to this vertex
PartnerList::Iterator PartnerList::get_least_preferred() {
    if (empty()) {
        return end();
    } else {
        RankType max_rank = get_rank(begin());
        Iterator max_it = begin();

        for (Iterator i = begin(), e = end(); i != e; ++i) {
            if (get_rank(i) > max_rank) {
                max_it = i;
                max_rank = get_rank(i);
            }
        }

        return max_it;
    }
}

void PartnerList::remove(VertexPtr v) {
    partners_.remove_if([v] (const PartnerType& p) { return p.second == v; });
}

/// remove the least preferred among the current partners
void PartnerList::remove_least_preferred() {
    Iterator it = get_least_preferred();

    if (it != end()) {
        partners_.erase(it);
    }
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

void PartnerList::sort() {
    partners_.sort();
}

std::ostream& operator<<(std::ostream& out, PartnerList& pl) {
    return out << &pl;
}

std::ostream& operator<<(std::ostream& out, PartnerList* pl) {
    std::stringstream stmp;

    for (auto i = pl->partners_.begin(), e = pl->partners_.end();
         i != e; ++i)
    {
        stmp << '(' << pl->get_vertex(i)->get_id() << ':'
             << pl->get_rank(i) << ')';
        stmp << ", ";
    }

    stmp.seekp(-2, stmp.cur);
    stmp << ';';
    return out << stmp.str();
}
