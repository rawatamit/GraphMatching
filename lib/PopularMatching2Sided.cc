#include "PopularMatching2Sided.h"
#include <queue>
#include <algorithm>

PopularMatching2Sided::PopularMatching2Sided(BipartiteGraph* G)
    : G_(G)
{
    find_matching();
}

PopularMatching2Sided::~PopularMatching2Sided() {
}

void PopularMatching2Sided::modify_graph() {
    // for each vertex in set A, add a copy with
    // level 1 and the same preference list
    for (Vertex* a : G_->get_A_partition()) {
        map_a1_[a->get_id()] = new Vertex(a->get_id(),
                                          a->get_level() + 1,
                                          a->get_preference_list());
    }
    
    // for every vertex in set B, add a copy of vertices
    // of level 1 in the same order
    for (Vertex* b : G_->get_B_partition()) {
        Vertex:: PreferenceListType& pref_list = b->get_preference_list();
        for (Vertex::PreferenceListReverseIterator
                i = b->preference_list_rbegin(),
                e = b->preference_list_rend();
             i != e; ++i)
        {
            // use vertices from level A
            pref_list.emplace_front(map_a1_[(*i)->get_id()]);
        }
    }
}

void PopularMatching2Sided::find_matching() {
   std::queue<Vertex*> Q;
   
   // insert level 0 vertices from set A in the queue
   std::for_each(G_->get_A_partition().begin(), G_->get_A_partition().end(),
                 [&Q] (Vertex* v) { Q.push(v); });
   modify_graph(); // update the graph with new vertices
   
   while (not Q.empty()) {
       // remove the first vertex from the queue
       // any arbitrary vertex will do
       Vertex* a = Q.front();
       Q.pop();
       
       // a0 has been rejected by all his neighbours,
       // now a1 enters, the preference list is the same as that of a0
       if (a->is_preferences_exhausted()) {
           // add a1 to the queue
           Q.push(map_a1_[a->get_id()]);
       } else {
           // b is the most preferred partner to a0
           Vertex* b = a->get_proposal_partner();
           
           // add M(b) to queue, if it exists
           if (b->is_matched()) {
               Q.push(b->get_partner());
           }
           
           // set M(b) to a0
           b->set_partner(a);
           
           // delete all partners of b worse than a0
           b->restrict_preference_list(a);
       }
   }
   
   // find the matched pairs
   for (Vertex* b : G_->get_B_partition()) {
       if (b->is_matched()) {
           matched_pair_list_.emplace_back(std::make_pair(b->get_partner()->get_id(), b->get_id()));
       }
   }
}

PopularMatching2Sided::MatchedPairListType const& PopularMatching2Sided::get_matched_pairs() const {
    return matched_pair_list_;
}
