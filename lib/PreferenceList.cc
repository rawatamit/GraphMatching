#include "PreferenceList.h"
#include "Vertex.h"
#include <algorithm>
#include <sstream>

PreferenceList::PreferenceList()
    : cur_rank_(0)
{}

PreferenceList::PreferenceList(const PreferenceList& that) {
    cur_rank_ = 0;
    pref_list_ = that.pref_list_;
}

PreferenceList::~PreferenceList() {
}

PreferenceList::Iterator PreferenceList::begin() {
    return pref_list_.begin();
}

PreferenceList::Iterator PreferenceList::end() {
    return pref_list_.end();
}

bool PreferenceList::empty() {
    return size() == 0;
}

PreferenceList::SizeType PreferenceList::size() {
    return end() - begin();
}

void PreferenceList::emplace_back(VertexPtr v) {
    pref_list_.emplace_back(++cur_rank_, v);
}

PreferenceList::Iterator PreferenceList::find(VertexPtr v) {
    for (auto i = begin(), e = end(); i != e; ++i) {
        if (i->vertex == v) {
            return i;
        }
    }

    return end();
}

PrefListElement PreferenceList::get(SizeType index) {
    return pref_list_.at(index);
}

std::ostream& operator<<(std::ostream& out, PreferenceList& pl) {
    return out << &pl;
}

std::ostream& operator<<(std::ostream& out, PreferenceList* pl) {
    std::stringstream stmp;

    for (PreferenceList::Iterator i = pl->begin(), e = pl->end();
         i != e; ++i)
    {
        stmp << i->vertex->get_id();

        if (i+1 == e) {
           stmp << ';';
        } else {
            stmp << ", ";
        }
    }

    return out << stmp.str();
}
