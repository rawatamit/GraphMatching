#ifndef VERTEX_H
#define VERTEX_H

#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include "PreferenceList.h"
#include "TDefs.h"

class Vertex {
private:
    IdType id_;                 // id of this vertex
    IdType cloned_for_id_;      // id of the vertex this was cloned for
    unsigned lower_quota_;           // least number of partners to this vertex
    unsigned upper_quota_;           // maximum number of partners to this vertex
    bool dummy_;                // is this a dummy vertex
    PreferenceList pref_list_;  // preference list according to priority
    PreferenceList pref_list_lq_;  // preference list containing LQ vertices

public:
    explicit Vertex(const IdType& id);
    explicit Vertex(const IdType& id, unsigned upper_quota);
    explicit Vertex(const IdType& id, unsigned lower_quota, unsigned upper_quota);
    explicit Vertex(const IdType& id, unsigned lower_quota, unsigned upper_quota, bool dummy);
    explicit Vertex(const IdType& id, const IdType& cloned_for_id, unsigned lower_quota,
                    unsigned upper_quota, bool dummy);
    virtual ~Vertex() = default;

    const IdType& get_id() const;
    const IdType& get_cloned_for_id() const;
    unsigned get_lower_quota() const;
    unsigned get_upper_quota() const;
    bool is_dummy() const;

    PreferenceList& get_preference_list();
    const PreferenceList& get_preference_list() const;

    PreferenceList& get_preference_list_lq();
    const PreferenceList& get_preference_list_lq() const;
};

/// print this vertex in the format v: p1, ..., pk
std::ostream& operator<<(std::ostream& out, VertexPtr v);

#endif
