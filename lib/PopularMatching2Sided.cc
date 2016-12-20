#include "PopularMatching2Sided.h"
#include "PreferenceList.h"
#include <queue>
#include <algorithm>
#include <iostream>

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
    for (BipartiteGraph::VertexType const& a : G_->get_A_partition()) {
        map_a1_.emplace(a->get_id(),
                        BipartiteGraph::VertexType(
                            new Vertex(a->get_id(),
                                       a->get_level() + 1,
                                       a->get_preference_list())));
    }
    
    // for every vertex in set B, add a copy of vertices
    // of level 1 in the same order
    // BUG: changes iterators !!!!!!
    for (BipartiteGraph::VertexType const& b : G_->get_B_partition()) {
        PreferenceList& pref_list = b->get_preference_list();
        for (PreferenceList::ReverseIterator
                i = pref_list.rbegin(),
                e = pref_list.rend();
             i != e; ++i)
        {
            // use vertices from level A
            pref_list.emplace_front(map_a1_[pref_list.get_vertex(i)->get_id()]);
        }
    }
    
    std::cout << G_;
}

void PopularMatching2Sided::find_matching() {
   std::queue<BipartiteGraph::VertexType> Q;
   
   // insert level 0 vertices from set A in the queue
   for (BipartiteGraph::VertexType const& a : G_->get_A_partition()) {
       Q.push(a);
   }
   
   // update the graph with new vertices
   modify_graph();
   
   while (not Q.empty()) {
       // remove the first vertex from the queue
       // any arbitrary vertex will do
       BipartiteGraph::VertexType const& a = Q.front();
       Q.pop();
       
       // a0 has been rejected by all his neighbours,
       // now a1 enters, the preference list is the same as that of a0
       if (a->is_preferences_exhausted()) {
           // add a1 to the queue
           Q.push(map_a1_[a->get_id()]);
       } else {
           // b is the most preferred partner to a0
           BipartiteGraph::VertexType const& b = a->get_proposal_partner();
           
           // add M(b) to queue, if it exists
           if (b->is_matched()) {
               Q.push(b->get_partner());
           }
           
           // set M(b) to a0
           b->set_partner(a);
           
           std::cout << "partner: " << b->get_id() << ": " << b->get_partner()->get_id() << '\n';
           
           // delete all partners of b worse than a0
           b->restrict_preference_list(a);
           
           // also remove b from a0's list
           a->remove(b);
       }
   }
   
   // find the matched pairs
   for (BipartiteGraph::VertexType const& b : G_->get_B_partition()) {
       std::cout << b << '\n';
       if (b->is_matched()) {
           matched_pair_list_.emplace_back(std::make_pair(b->get_partner()->get_id(), b->get_id()));
       }
   }
}

PopularMatching2Sided::MatchedPairListType const& PopularMatching2Sided::get_matched_pairs() const {
    return matched_pair_list_;
}
