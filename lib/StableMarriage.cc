#include "StableMarriage.h"
#include "Vertex.h"
#include <stack>

StableMarriage::StableMarriage(const std::unique_ptr<BipartiteGraph>& G,
                               bool A_proposing)
    : MatchingAlgorithm(G), A_proposing_(A_proposing)
{}

StableMarriage::~StableMarriage()
{}

#include <iostream>
void StableMarriage::compute_matching() {
    std::stack<VertexPtr> free_list;
    const std::unique_ptr<BipartiteGraph>& G = get_graph();

    // choose the paritions from which the vertices will propose
    // we will assume the proposing side vertices are called men
    // and the vertices from the other partition women
    const auto& proposing_parition = G->get_A_partition(); // : G->get_B_partition();

    // mark all proposing vertices (by pushing into the free_list)
    // and vertices from the opposite partition (implicitly) free
    for (auto it : proposing_parition) {
        free_list.push(it.second);
    }

    while (not free_list.empty()) {
        auto m = free_list.top();
        auto& m_pref_list = m->get_preference_list();
        free_list.pop(); // remove m from free_list

        // if the preferences of m have not been exhausted
        if (not m_pref_list.empty()) {
            // highest ranked woman to whom m has not yet proposed
            auto w = m_pref_list.get_vertex(m_pref_list.get_proposal_index());

            // w's preference list and list of partners
            auto& w_pref_list = w->get_preference_list();
            auto& w_partner_list = w->get_partners();

            // m's rank on w's preference list
            auto m_rank = w_pref_list.get_rank(w_pref_list.find(m));

            // w's rank on m's preference list
            auto w_rank = m_pref_list.get_rank(m_pref_list.get_proposal_index());

            if (w_partner_list.size() == w->get_upper_quota()) {
                // w's least preferred partner
                auto worst_partner = w_partner_list.get_least_preferred();

                // worst partners rank, details, and preference list
                auto mc = w_partner_list.get_vertex(worst_partner);
                auto mc_rank = w_partner_list.get_rank(worst_partner);
                auto& mc_pref_list = mc->get_preference_list();
                auto& mc_partner_list = mc->get_partners();

#if 0
if (m->get_id() == "r106") { std::cerr << w->get_id() << ' '
  << w->get_upper_quota() << ' ' << w_partner_list.size() << ' '
    << m->get_id() << ' ' << m_rank << ' ' << mc->get_id() << ' ' << mc_rank << '\n';
  std::cerr << "P: " << w_partner_list << '\n';
  std::cerr << "PR: " << w_pref_list << '\n';
}
#endif

                // does w prefer m over its worst partner?
                if (m_rank < mc_rank) {
                    // remove mc from w's matched partner list
                    w_partner_list.remove_least_preferred();

                    // add (m, w) in the matching
                    w->add_partner(std::make_pair(m_rank, m));
                    m->add_partner(std::make_pair(w_rank, w));

                    // mark mc free
                    mc_partner_list.remove_least_preferred();

                    // remove w from mc's preferences
                    mc_pref_list.remove_first(); 

                    // push mc to free list
                    free_list.push(mc);
                } else {
                    // remove w from m's preferences
                    m_pref_list.remove_first();
                    free_list.push(m); // m remains unmatched
                }

                //w_pref_list.restrict_preferences(m);
            } else { // w is free
                // accept the proposal
                w->add_partner(std::make_pair(m_rank, m));
                m->add_partner(std::make_pair(w_rank, w));

//                // add m to the preference list if it has residual capacity
//                if (m->get_upper_quota() > m_pref_list.size()) {
//                  free_list.push(m);
//                }
            }
        }
    }
}

