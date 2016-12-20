#include "Vertex.h"
#include <sstream>

Vertex::Vertex(IdType const& id)
    : id_(id)
{}

Vertex::Vertex(IdType const& id,
               PreferenceList const& pref_list)
    : id_(id), pref_list_(pref_list)
{}

Vertex::~Vertex() {
    if (is_matched()) {
        partner_.reset();
    }
    pref_list_.clear();
}

Vertex::IdType const& Vertex::get_id() const {
    return id_;
}

const Vertex::VertexType& Vertex::get_partner() const {
    return partner_;
}

void Vertex::set_partner(const VertexType& partner) {
    partner_ = partner;
}

PreferenceList& Vertex::get_preference_list() {
    return pref_list_;
}

PreferenceList const& Vertex::get_preference_list() const {
    return pref_list_;
}

/// is this vertex matched to someone else
bool Vertex::is_matched() {
    return get_partner() != nullptr;
}

/// print this vertex in the format v: p1, p2
std::ostream& operator<<(std::ostream& out, Vertex* v) {
    std::stringstream stmp;

    stmp << v->get_id() << ": "; // print vertex id
    stmp << v->get_preference_list() << '\n';

#if 0
    for (PreferenceList::Iterator
            i = pref_list.begin(),
            e = pref_list.end();
        i != e; ++i)
    {
        PreferenceList::Iterator tmpiter = i;

        stmp << "(" << pref_list.get_vertex(i)->get_id() << ") ";
        if (++tmpiter != e) {
            stmp << ' ';
        } else {
            stmp << '\n';
        }
    }
#endif

    return out << stmp.str();
}
