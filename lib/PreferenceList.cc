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
    for (auto i = cbegin(); i != cend(); ++i) {
        // check whether the particular rank is tied
        if (i->vertex != nullptr) {
            // if rank is not tied, 
            // compare it to the vertex v
            if (i->vertex == v) {
                return (SizeType) std::distance(cbegin(), i);
            }
        } else {
            // if the rank is tied, 
            // search the tied list for v
            auto index = std::distance(cbegin(), i);
            auto tied_list = get_ties((RankType) index);
            for (auto j = tied_list.begin(), end = tied_list.end(); j != end; ++j) {
                if (j->vertex == v) {
                    return (SizeType) index;
                }
            }
        }
    }
    return (SizeType) std::distance(cbegin(), cend());
}

PrefListElement PreferenceList::at(SizeType index) const {
    return pref_list_.at(index);
}

void PreferenceList::set_tie(RankType rank, VertexPtr v) {
    if(ties_.find(rank) == ties_.end()) {
        ++cur_rank_;
    }
    ties_[rank].emplace_back(rank, v);
}

const std::vector<PrefListElement>& PreferenceList::get_ties(RankType rank) const {
    return ties_.at(rank);
}

bool PreferenceList::is_tied(RankType rank) const {
    return (ties_.find(rank) != ties_.end());
}

// Does b prefer a1 over a2?
PreferenceOrderT PreferenceList::prefers(VertexPtr a1, VertexPtr a2) { 
  // Return 0 if rank of a1 = rank of a2
  // Return 1 if rank of a1 < rank of a2
  // Return 2 o.w.
  auto a1_rank = (RankType)find_index(a1);
  auto a2_rank = (RankType)find_index(a2);
  if(a1_rank < a2_rank) {
    return cBetter;
  }
  else if(a1_rank == a2_rank) {
    return cEqual;
  }
  else {
    return cWorse;
  }
}

std::ostream& operator<<(std::ostream& out, const PreferenceList& pl) {
    return out << &pl;
}

std::ostream& operator<<(std::ostream& out, const PreferenceList* pl) {
    std::stringstream stmp;

    for (auto i = pl->cbegin(), e = pl->cend(); i != e; ++i) {
        if (i->vertex != nullptr) {
            stmp << i->vertex->get_id();
        } else {
            auto index = std::distance(i, pl->cbegin());
            stmp << "(";
            auto tied_list = pl->get_ties(index);
            for(auto j = tied_list.begin(), end = tied_list.end(); j != end; ++j) {
                stmp << j->vertex->get_id();
                if ((j+1) != end) {
                    stmp << ", ";
                }
            }
            stmp << ")";
        }
        if (i+1 == e) {
            stmp << ';';
        } else {
            stmp << ", ";
        }
    }

    return out << stmp.str();
}
