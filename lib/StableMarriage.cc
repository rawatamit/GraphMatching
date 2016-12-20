#include "StableMarriage.h"
#include "Vertex.h"
#include <stack>

StableMarriage::StableMarriage(const std::unique_ptr<BipartiteGraph>& G) :
    MatchingAlgorithm(G)
{}

StableMarriage::~StableMarriage()
{}

void StableMarriage::compute_matching() {
    std::stack<BipartiteGraph::VertexType> free_list;
    const std::unique_ptr<BipartiteGraph>& G = get_graph();

    // mark all men (by pushing into the free_list) and women (implicitly) free
    for (auto m : G->get_A_partition()) {
        free_list.push(m);
    }

    while (not free_list.empty()) {
        auto m = free_list.top();
        auto& m_pref_list = m->get_preference_list();
        free_list.pop(); // remove m from free_list

        // if the preferences of m have not been exhausted
        if (not m_pref_list.empty()) {
            // highest ranked woman to whom m has not yet proposed
            auto w = m_pref_list.get_proposal_partner();
            auto& w_pref_list = w->get_preference_list();

            if (w->is_matched()) {
                // w's current matched partner
                auto m_ = w->get_partner();

                // does w prefer m over m_ ?
                // if yes match w with m, and mark m_ free
                if (w_pref_list.is_ranked_better(m, m_)) {
                    w->set_partner(m);
                    m->set_partner(w);

                    // mark m_ free and push to free_list
                    m_->set_partner(nullptr);
                    free_list.push(m_);
                } else {
                    free_list.push(m); // m remains unmatched
                }
            } else { // w is free
                // accept the proposal
                w->set_partner(m);
                m->set_partner(w);
            }

            m_pref_list.remove_first(); // remove w from m's preferences
            // remove all lower ranked partners than m
            w_pref_list.restrict_preferences(m);
        }
    }

    // finish off the pending work
    MatchingAlgorithm::compute_matching();
}

