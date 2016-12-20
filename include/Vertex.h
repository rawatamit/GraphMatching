#ifndef VERTEX_H
#define VERTEX_H

#include <ostream>
#include <string>
#include "PreferenceList.h"

class Vertex {
public:
    typedef std::string IdType;
    typedef std::shared_ptr<Vertex> VertexType;

protected:
    IdType id_;  // id of this vertex
    VertexType
        partner_;  // current partner of this vertex, nullptr if not matched
    PreferenceList pref_list_;  // preference list according to priority

public:
    Vertex(IdType const& id);
    Vertex(IdType const& id, PreferenceList const& pref_list);
    virtual ~Vertex();

    IdType const& get_id() const;
    const VertexType& get_partner() const;
    void set_partner(const VertexType& partner);
    // void set_preference_list(PreferenceList const& preference_list);

    PreferenceList& get_preference_list();
    PreferenceList const& get_preference_list() const;

    /// is this vertex matched to someone else
    bool is_matched();

    /// print this vertex in the format v: p1, p2
    friend std::ostream& operator<<(std::ostream& out, Vertex* v);
};

#endif
