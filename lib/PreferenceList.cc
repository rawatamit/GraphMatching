#include "PreferenceList.h"
#include "Vertex.h"
#include <algorithm>
#include <sstream>

PreferenceList::PreferenceList()
    : start_iter_(0), end_iter_(0), cur_rank_(0)
{}

PreferenceList::PreferenceList(const PreferenceList& that) {
    cur_rank_ = that.cur_rank_;
    pref_list_ = that.pref_list_;
    start_iter_ = 0;
    end_iter_ = pref_list_.size();
}

PreferenceList& PreferenceList::operator=(const PreferenceList& that) {
    if (this != &that) {
        clear(); // empty the preference list
        cur_rank_ = that.cur_rank_;
        pref_list_ = that.pref_list_;
        start_iter_ = 0;
        end_iter_ = pref_list_.size();
    }

    return *this;
}

PreferenceList::~PreferenceList() {
    clear();
}

/// find the vertex in the container
PreferenceList::SizeType PreferenceList::find(const VertexType& v) {
    for (SizeType i = begin(), e = end(); i != e; ++i) {
        if (get_vertex(pref_list_.at(i)) == v) {
            return i;
        }
    }

    return end();
}

PreferenceList::SizeType PreferenceList::begin() {
    return start_iter_;
}

PreferenceList::SizeType PreferenceList::end() {
    return end_iter_;
}

/// clear the list
void PreferenceList::clear() {
    pref_list_.clear();
    start_iter_ = 0;
    end_iter_ = 0;
}

/// is the preference list empty
bool PreferenceList::empty() {
    return size() == 0;
}

/// is the preference list empty
PreferenceList::SizeType PreferenceList::size() {
    return end() - begin();
}

/// insert element at end
void PreferenceList::emplace_back(const VertexType& v) {
    pref_list_.emplace_back(++cur_rank_, v);

    // end_iter_ always points one past the end of preference list
    ++end_iter_;
}

PreferenceList::VertexType PreferenceList::get_vertex(const ElementType& e) {
    return e.second;
}

PreferenceList::RankType PreferenceList::get_rank(const ElementType& e) {
    return e.first;
}

PreferenceList::VertexType PreferenceList::get_vertex(SizeType index) {
    return pref_list_.at(index).second;
}

PreferenceList::RankType PreferenceList::get_rank(SizeType index) {
    return (index >= end()) ? RANK_INFINITY : pref_list_.at(index).first;
}

/// return the vertex this vertex will now propose to
PreferenceList::VertexType PreferenceList::get_proposal_partner() {
    return get_vertex(begin());
}

/// does this vertex prefer a to b
bool PreferenceList::is_ranked_better(const VertexType& a, const VertexType& b) {
    return get_rank(find(a)) < get_rank(find(b));
}

/// restrict the preference list [begin(), pref_list_[v]]
void PreferenceList::restrict_preferences(const VertexType& v) {
    auto index = find(v);
    end_iter_ = (index != end()) ? (index + 1) : end();
}

/// remove the first vertex from the preference list
void PreferenceList::remove_first() {
    ++start_iter_;
}

std::ostream& operator<<(std::ostream& out, PreferenceList& pl) {
    return out << &pl;
}

std::ostream& operator<<(std::ostream& out, PreferenceList* pl) {
    std::stringstream stmp;

    for (PreferenceList::SizeType i = pl->begin(), e = pl->end();
         i != e; ++i)
    {
        stmp << pl->get_vertex(pl->pref_list_.at(i))->get_id() << ' ';
    }

    return out << stmp.str();
}

