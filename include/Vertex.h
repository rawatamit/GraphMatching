#ifndef VERTEX_H
#define VERTEX_H

#include <string>
#include <queue>
#include <ostream>

class Vertex {
public:
    typedef std::string IdType;
    typedef std::deque<Vertex*> PreferenceListType;
    typedef PreferenceListType::iterator PreferenceListIterator;
    typedef PreferenceListType::reverse_iterator PreferenceListReverseIterator;
    typedef PreferenceListType::size_type PreferenceListIndex;

protected:
    IdType id_; // id of this vertex
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
    Vertex(IdType const& id, int level = 0);
    Vertex(IdType const& id,
           int level,
           PreferenceListType const& preference_list);
    virtual ~Vertex();
    
    PreferenceListIterator preference_list_begin();
    PreferenceListIterator preference_list_end();
    PreferenceListReverseIterator preference_list_rbegin();
    PreferenceListReverseIterator preference_list_rend();

    IdType const& get_id() const;
    Vertex* get_partner();
    void set_partner(Vertex* partner);
    void set_preference_list(PreferenceListType const& preference_list);
    
    int get_level();
    PreferenceListType& get_preference_list();
    PreferenceListType const& get_preference_list() const;
    
    /// returns the partner that this vertex will propose
    Vertex* get_proposal_partner();
    
    /// is this vertex matched to someone else
    bool is_matched();
    
    /// has this vertex proposed to everyone in his preference list
    bool is_preferences_exhausted();
    
    /// true if this vertex prefers a to b
    bool prefers(Vertex* a, Vertex* b);
    
    /// restrict the preference list upto vertex v
    void restrict_preference_list(Vertex* v);
    
    /// print this vertex in the format v: p1, p2
    friend std::ostream& operator<<(std::ostream& out, Vertex* v);
};

#endif
