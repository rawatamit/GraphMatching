#include "Vertex.h"
#include <algorithm>
#include <sstream>

Vertex::Vertex(IdType const& id, int level)
    : id_(id), level_(level)
{
}

Vertex::Vertex(IdType const& id,
       int level,
       PreferenceListType const& preference_list)
    : id_(id), level_(level)
{
    set_preference_list(preference_list);
}

Vertex::~Vertex() {
    preference_list_.clear();
}

Vertex::PreferenceListIterator Vertex::preference_list_begin() {
    return preference_list_.begin();
}

Vertex::PreferenceListIterator Vertex::preference_list_end() {
    return preference_list_.end();
}

Vertex::PreferenceListReverseIterator Vertex::preference_list_rbegin() {
    return preference_list_.rbegin();
}

Vertex::PreferenceListReverseIterator Vertex::preference_list_rend() {
    return preference_list_.rend();
}

Vertex::PreferenceListIterator Vertex::find_vertex(Vertex* v) {
    return std::find_if(preference_list_.begin(),
                        proposal_iter_,
                        [v](Vertex* a) { return a->get_id() == v->get_id(); });
}

Vertex::IdType const& Vertex::get_id() const {
    return id_;
}

Vertex* Vertex::get_partner() {
    return partner_;
}

void Vertex::set_partner(Vertex* partner) {
    partner_ = partner;
}

void Vertex::set_preference_list(PreferenceListType const& preference_list) {
    preference_list_ = preference_list;
    current_proposal_index_ = 0;
    proposal_iter_ = preference_list_.end();
}

int Vertex::get_level() {
    return level_;
}

Vertex::PreferenceListType& Vertex::get_preference_list() {
    return preference_list_;
}

Vertex::PreferenceListType const& Vertex::get_preference_list() const {
    return preference_list_;
}

/// returns the partner that this vertex will propose
Vertex* Vertex::get_proposal_partner() {
    return preference_list_.at(current_proposal_index_++);
}

/// is this vertex matched to someone else
bool Vertex::is_matched() {
    return get_partner() != nullptr;
}

/// has this vertex proposed to everyone in his preference list
bool Vertex::is_preferences_exhausted() {
    return current_proposal_index_ == preference_list_.size();
}

/// true if this vertex prefers a to b
bool Vertex::prefers(Vertex* a, Vertex* b) {
    return find_vertex(a) < find_vertex(b);
}

/// restrict the preference list upto vertex v
void Vertex::restrict_preference_list(Vertex* v) {
    // set the proposal_iter_, a bit tricky
    PreferenceListIterator iter = find_vertex(v);
    proposal_iter_ = iter == preference_list_.end() ? iter : iter + 1;
}

/// print this vertex in the format v: p1, p2
std::ostream& operator<<(std::ostream& out, Vertex* v) {
    std::stringstream stmp;
    stmp << v->get_id() << ": ";
    
    for (Vertex::PreferenceListIterator
            i = v->preference_list_begin(),
            e = v->preference_list_end();
        i != e; ++i)
    {
        stmp << "(" << (*i)->get_id() << ", " << (*i)->get_level() << ") ";
        if (i + 1 != e) {
            stmp << ' ';
        } else {
            stmp << '\n';
        }
    }
    
    return out << stmp.str();
}
