#include "PreferenceList.h"
#include "Vertex.h"
#include <algorithm>
#include <sstream>
#include <iostream>

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
    return (SizeType) std::distance(cbegin(), find(v));
}

PrefListElement PreferenceList::at(SizeType index) const {
    return pref_list_.at(index);
}

void PreferenceList::set_ties(RankType rank, PreferenceList p){
    SizeType size = p.size();
    cur_rank_++;
    is_tied_[rank] = true;
    for(SizeType i=0; i<size; i++){
        tie_map_[rank].emplace_back(rank, p.at(i).vertex); //the rank for the preference list element is index 
    }
}

PreferenceList::ContainerType PreferenceList::get_ties(RankType rank) const {
    return tie_map_.at(rank);
}

bool PreferenceList::isTied(RankType rank) const {
    return is_tied_.at(rank);
}

void PreferenceList::printList(){
    for (auto i = this->cbegin(), e = this->cend(); i != e; ++i) {
        if(i->vertex != nullptr){
            std::cout << i->vertex->get_id() << " ";
        }
        else{
            int index = i-this->cbegin();
            std::cout << "( ";
            for(auto j: get_ties(index)){
                std::cout << j.vertex->get_id() << " ";
            }
            std::cout << ")";
        }
    }
    std::cout << std::endl;
}

std::ostream& operator<<(std::ostream& out, const PreferenceList& pl) {
    return out << &pl;
}

std::ostream& operator<<(std::ostream& out, const PreferenceList* pl) {
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
