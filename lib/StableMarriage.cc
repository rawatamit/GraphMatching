#include "StableMarriage.h"
#include "Vertex.h"
#include "PartnerList.h"
#include <stack>

StableMarriage::StableMarriage(const std::unique_ptr<BipartiteGraph>& G,
                               bool A_proposing)
    : MatchingAlgorithm(G), A_proposing_(A_proposing)
{}

StableMarriage::~StableMarriage()
{}

bool StableMarriage::compute_matching() {
    std::stack<VertexPtr> free_list;
    const std::unique_ptr<BipartiteGraph>& G = get_graph();

    // choose the paritions from which the vertices will propose
    const auto& proposing_partition = A_proposing_ ? G->get_A_partition()
                                                   : G->get_B_partition();

    // mark all proposing vertices (by pushing into the free_list)
    // and vertices from the opposite partition (implicitly) free
    for (auto it : proposing_partition) {
        free_list.push(it.second);
    }

    while (not free_list.empty()) {
        auto u = free_list.top();
        auto& u_pref_list = u->get_preference_list();
        auto& u_partner_list = M_[u];
        free_list.pop(); // remove u from free_list

        // if the preferences of u have not been exhausted
        if (not u_pref_list.empty()) {
            // highest ranked vertex to whom u not yet proposed
            auto v = u_pref_list.get_vertex(u_pref_list.get_proposal_index());

            // v's preference list and list of partners
            auto& v_pref_list = v->get_preference_list();
            auto& v_partner_list = M_[v];

            // u's rank on v's preference list
            auto u_rank = v_pref_list.get_rank(v_pref_list.find(u));

            // v's rank on u's preference list
            auto v_rank = u_pref_list.get_rank(u_pref_list.get_proposal_index());

            if (v_partner_list.size() == v->get_upper_quota()) {
                // v's least preferred partner
                auto worst_partner = v_partner_list.get_least_preferred();

                // worst partners rank, and preference list
                auto uc = v_partner_list.get_vertex(worst_partner);
                auto uc_rank = v_partner_list.get_rank(worst_partner);
                auto& uc_pref_list = uc->get_preference_list();
                auto& uc_partner_list = M_[uc];

                // does v prefer u over its worst partner?
                if (u_rank < uc_rank) {
                    // remove uc from v's matched partner list
                    v_partner_list.remove_least_preferred();

                    // add (u, v) in the matching
                    v_partner_list.add_partner(std::make_pair(u_rank, u));
                    u_partner_list.add_partner(std::make_pair(v_rank, v));

                    // mark uc free
                    uc_partner_list.remove_least_preferred();

                    // remove v from uc's preferences
                    uc_pref_list.remove_first(); 

                    // push uc to free list
                    if (not uc_pref_list.empty()) {
                        free_list.push(uc);
                    }
                }

                //v_pref_list.restrict_preferences(u);
            } else { // v is free
                // accept the proposal
                v_partner_list.add_partner(std::make_pair(u_rank, u));
                u_partner_list.add_partner(std::make_pair(v_rank, v));
            }

            // add u to the free list if it has residual capacity
            // and its preference list is not empty
            if (not u_pref_list.empty() and
                u->get_upper_quota() > u_partner_list.size())
            {
                // remove v from u's preferences
                u_pref_list.remove_first();
                free_list.push(u);
            }
        }
    }

    return true;
}

