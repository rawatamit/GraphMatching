#include "Vertex.h"
#include <sstream>

Vertex::Vertex(const IdType& id)
    : Vertex(id, 0, 1, false)
{}

Vertex::Vertex(const IdType& id, int upper_quota)
    : Vertex(id, 0, upper_quota, false)
{}

Vertex::Vertex(const IdType& id, int lower_quota, int upper_quota)
    : Vertex(id, lower_quota, upper_quota, false)
{}

Vertex::Vertex(const IdType& id, int lower_quota, int upper_quota, bool dummy)
    : id_(id),
      lower_quota_(lower_quota), upper_quota_(upper_quota),
      dummy_(dummy)
{}

Vertex::~Vertex()
{}

IdType const& Vertex::get_id() const {
    return id_;
}

int Vertex::get_lower_quota() const {
    return lower_quota_;
}

int Vertex::get_upper_quota() const {
    return upper_quota_;
}

bool Vertex::is_dummy() const {
    return dummy_;
}

const PartnerList& Vertex::get_partners() const {
    return partners_;
}

PartnerList& Vertex::get_partners() {
    return partners_;
}

void Vertex::add_partner(const PartnerList::PartnerType& partner) {
    partners_.add_partner(partner);
}

PreferenceList& Vertex::get_preference_list() {
    return pref_list_;
}

PreferenceList const& Vertex::get_preference_list() const {
    return pref_list_;
}

/// does this vertex have at least a partner
bool Vertex::has_partner() {
    return get_partners().size() != 0;
}

/// print this vertex in the format v: p1, ..., pk
std::ostream& operator<<(std::ostream& out, VertexPtr v) {
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
