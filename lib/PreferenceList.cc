#include "PreferenceList.h"
#include "Vertex.h"
#include <algorithm>
#include <sstream>

PreferenceList::PreferenceList()
    : cur_rank_(0)
{}

PreferenceList::Iterator PreferenceList::begin() {
    return pref_list_.begin();
}

PreferenceList::Iterator PreferenceList::end() {
    return pref_list_.end();
}

PreferenceList::ConstIterator PreferenceList::cbegin() const {
    return pref_list_.cbegin();
}

PreferenceList::ConstIterator PreferenceList::cend() const {
    return pref_list_.cend();
}

bool PreferenceList::empty() {
    return pref_list_.empty();
}

PreferenceList::SizeType PreferenceList::size() const {
    return pref_list_.size();
}

void PreferenceList::emplace_back(VertexPtr v) {
    pref_list_.emplace_back(++cur_rank_, v);
}

PreferenceList::ConstIterator PreferenceList::find(VertexPtr v) const {
    return std::find_if(cbegin(), cend(), [&v] (const PrefListElement& i) { return i.vertex == v; });
}

PreferenceList::SizeType PreferenceList::find_index(VertexPtr v) const {
    return (SizeType) std::distance(cbegin(), find(std::move(v)));
}

PrefListElement PreferenceList::at(SizeType index) const {
    return pref_list_.at(index);
}

std::ostream& operator<<(std::ostream& out, PreferenceList& pl) {
    return out << &pl;
}

std::ostream& operator<<(std::ostream& out, PreferenceList* pl) {
    std::stringstream stmp;

    for (auto i = pl->cbegin(), e = pl->cend(); i != e; ++i) {
        stmp << i->vertex->get_id();

        if (i+1 == e) {
           stmp << ';';
        } else {
            stmp << ", ";
        }
    }

    return out << stmp.str();
}
