#include "PartnerList.h"
#include "Vertex.h"
#include <algorithm>
#include <sstream>

PartnerList::PartnerList()
{}

PartnerList::~PartnerList() {
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
        if (i->partner == v) {
            return i;
        }
    }

    return cend();
}

#include <iostream>
void PartnerList::add_partner(VertexPtr partner, RankType rank, int level) {
//auto size_before = partners_.size();
   partners_.emplace(partner, rank, level);
//std::cout << "psize before: " << size_before << ", after: " << partners_.size() << '\n';
}

Partner PartnerList::get_least_preferred() {
    return *partners_.begin();//*std::prev(partners_.end());
}

void PartnerList::remove(VertexPtr v) {
    for (auto i = partners_.begin(), e = partners_.end(); i != e; ++i) {
        if (i->partner == v) {
//std::cout << "removing: " << i->partner->get_id() << '\n';
            partners_.erase(i);
            break;
        }
    }
}

/*
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
*/