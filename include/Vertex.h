#ifndef VERTEX_H
#define VERTEX_H

#include <string>
#include <vector>

class Vertex {
public:
    typedef std::vector<Vertex*> PreferenceListType;
    typedef PreferenceListType::iterator PreferenceListIterator;
    typedef PreferenceListType::size_type PreferenceListIndex;

protected:
    std::string id_; // id of this vertex
    int level_; // level of the vertex
    Vertex* partner_; // current partner of this vertex, nullptr if not matched
    PreferenceListType preference_list_; // preference list according to priority
    PreferenceListIndex current_proposal_index_; // the vertex that this vertex will propose
    PreferenceListIterator proposal_iter_; // iterator to one past the last vertex that this vertex can propose to

private:
    /// finds and returns the location of the vertex v
    /// in the preference list, returns PreferenceListType::end()
    /// if v is not present in the list
    PreferenceListIterator find_vertex(Vertex* v);

public:
    Vertex(std::string const& id,
           int level,
           PreferenceListType const& preference_list);
    virtual ~Vertex();
    
    std::string const& get_id();
    Vertex* get_partner();
    void set_partner(Vertex* partner);
    
    /// true if this vertex prefers a to b
    bool prefers(Vertex* a, Vertex* b);
    
    /// restrict the preference list upto vertex v
    void restrict_preference_list(Vertex* v);
};

#endif
