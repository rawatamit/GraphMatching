#ifndef VERTEX_H
#define VERTEX_H

#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include "PartnerList.h"
#include "PreferenceList.h"
#include "TDefs.h"

class Vertex {
private:
    IdType id_;                 // id of this vertex
    IdType cloned_for_id_;      // id of the vertex this was cloned for
    int lower_quota_;           // least number of partners to this vertex
    int upper_quota_;           // maximum number of partners to this vertex
    bool dummy_;                // is this a dummy vertex
    PartnerList partners_;      // partners of this vertex, empty if not matched
    PreferenceList pref_list_;  // preference list according to priority

public:
    Vertex(const IdType& id);
    Vertex(const IdType& id, int upper_quota);
    Vertex(const IdType& id, int lower_quota, int upper_quota);
    Vertex(const IdType& id, int lower_quota, int upper_quota, bool dummy);
    Vertex(const IdType& id, const IdType& cloned_for_id, int lower_quota,
           int upper_quota, bool dummy);
    virtual ~Vertex();

    const IdType& get_id() const;
    const IdType& get_cloned_for_id() const;
    int get_lower_quota() const;
    int get_upper_quota() const;
    bool is_dummy() const;
    const PartnerList& get_partners() const;
    PartnerList& get_partners();
    void add_partner(const PartnerList::PartnerType& partner);

    PreferenceList& get_preference_list();
    PreferenceList const& get_preference_list() const;

    /// does this vertex have at least a partner
    bool has_partner();
};

/// print this vertex in the format v: p1, ..., pk
std::ostream& operator<<(std::ostream& out, VertexPtr v);

#endif
