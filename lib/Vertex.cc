#include "Vertex.h"
#include <sstream>

Vertex::Vertex(const IdType& id)
    : Vertex(id, id, 0, 1, false, false)
{}

Vertex::Vertex(const IdType& id, unsigned upper_quota)
    : Vertex(id, id, 0, upper_quota, false, false)
{}

Vertex::Vertex(const IdType& id, unsigned lower_quota, unsigned upper_quota)
    : Vertex(id, id, lower_quota, upper_quota, false, false)
{}

Vertex::Vertex(const IdType& id, unsigned lower_quota, unsigned upper_quota,
               bool dummy, bool last_resort)
    : Vertex(id, id, lower_quota, upper_quota, dummy, last_resort)
{}

Vertex::Vertex(const IdType& id, const IdType& cloned_for_id,
               unsigned lower_quota, unsigned upper_quota, bool dummy,
               bool last_resort)
    : id_(id), cloned_for_id_(cloned_for_id),
      lower_quota_(lower_quota), upper_quota_(upper_quota),
      dummy_(dummy), last_resort_(last_resort)
{}

IdType const& Vertex::get_id() const {
    return id_;
}

IdType const& Vertex::get_cloned_for_id() const {
    return cloned_for_id_;
}

unsigned Vertex::get_lower_quota() const {
    return lower_quota_;
}

unsigned Vertex::get_upper_quota() const {
    return upper_quota_;
}

bool Vertex::is_dummy() const {
    return dummy_;
}

bool Vertex::is_last_resort() const {
    return last_resort_;
}

PreferenceList& Vertex::get_preference_list() {
    return pref_list_;
}

PreferenceList const& Vertex::get_preference_list() const {
    return pref_list_;
}

PreferenceList& Vertex::get_preference_list_lq() {
    return pref_list_lq_;
}

PreferenceList const& Vertex::get_preference_list_lq() const {
    return pref_list_lq_;
}

/// print this vertex in the format v: p1, ..., pk
std::ostream& operator<<(std::ostream& out, VertexPtr v) {
    std::stringstream stmp;
    stmp << v->get_id() << ": "; // print vertex id
    stmp << v->get_preference_list() << '\n';
    return out << stmp.str();
}
