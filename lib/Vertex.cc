#include "Vertex.h"
#include <algorithm>

Vertex::Vertex(std::string const& id,
       int level,
       PreferenceListType const& preference_list)
    : id_(id), level_(level), preference_list_(preference_list)
{
    current_proposal_index_ = 0;
    proposal_iter_ = preference_list_.end();
}

Vertex::~Vertex() {
    preference_list_.clear();
}

Vertex::PreferenceListIterator Vertex::find_vertex(Vertex* v) {
    return std::find_if(preference_list_.begin(),
                        proposal_iter_,
                        [v](Vertex* a) { return a->get_id() == v->get_id(); });
}

std::string const& Vertex::get_id() {
    return id_;
}

Vertex* Vertex::get_partner() {
    return partner_;
}

void Vertex::set_partner(Vertex* partner) {
    partner_ = partner;
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
