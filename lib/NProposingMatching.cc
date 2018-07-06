#include "NProposingMatching.h"
#include "Vertex.h"
#include "PartnerList.h"
#include <stack>
#include <map>

NProposingMatching::NProposingMatching(const std::unique_ptr<BipartiteGraph>& G,
                                       bool A_proposing, int max_level)
    : MatchingAlgorithm(G), A_proposing_(A_proposing), max_level(max_level)
{}

NProposingMatching::~NProposingMatching()
{}

#include <iostream>
bool NProposingMatching::compute_matching() {
    std::stack<VertexPtr> free_list;
    std::map<VertexPtr, int> in_queue;
    std::map<VertexPtr, int> vertex_level;
    std::map<VertexPtr, PreferenceList::SizeType> proposal_index;
    const std::unique_ptr<BipartiteGraph>& G = get_graph();

    // choose the paritions from which the vertices will propose
    const auto& proposing_partition = A_proposing_ ? G->get_A_partition()
                                                   : G->get_B_partition();

    // set the level of every vertex in the proposing partition to 0
    // mark all proposing vertices free (by pushing into the free_list)
    // and vertices from the opposite partition implicitly free
    for (auto it : proposing_partition) {
        free_list.push(it.second);
        in_queue[it.second] = 1;
        vertex_level[it.second] = 0;
        proposal_index[it.second] = 0;
    }

    // there is at least one vertex in the free list
    while (not free_list.empty()) {
        // first vertex in free list 
        auto u = free_list.top();
        auto& u_pref_list = u->get_preference_list();
        auto& u_partner_list = M_[u];
        free_list.pop(); // remove u from free_list
        in_queue[u] = 0; // u is not in free list now

        // preferences of u have not been exhausted
        if (vertex_level[u] < max_level + 1) {
            // highest ranked vertex to whom u not yet proposed
            auto v = u_pref_list.get_vertex(proposal_index[u]);

            // v's preference list and list of partners
            auto& v_pref_list = v->get_preference_list();
            auto& v_partner_list = M_[v];

            // u's rank on v's preference list
            auto u_rank = v_pref_list.get_rank(v_pref_list.find(u));

            // v's rank on u's preference list
            auto v_rank = u_pref_list.get_rank(proposal_index[u]);

            if (v_partner_list.size() == v->get_upper_quota()) {
                // v's least preferred partner
                auto worst_partner = v_partner_list.get_least_preferred();

                // worst partners rank, and matched partners
                auto uc = v_partner_list.get_vertex(worst_partner);
                auto uc_rank = v_partner_list.get_rank(worst_partner);
                auto& uc_partner_list = M_[uc];

                // does v prefer u over its worst partner?
                // this could be either because v's worst partner
                // has a lower level than u or has a higher rank than u 
                if (vertex_level[uc] < vertex_level[u] or u_rank < uc_rank) {
                    // remove uc from v's matched partner list
                    v_partner_list.remove_least_preferred();

                    // add (u, v) in the matching
                    u_partner_list.add_partner(std::make_pair(v_rank, v));
                    v_partner_list.add_partner(std::make_pair(u_rank, u));

                    // mark uc free
                    uc_partner_list.remove(v);

                    // push uc to free list
                    if (in_queue[uc] == 0) {
                        free_list.push(uc);
                        in_queue[uc] = 1;
                    }
                }
            } else {
                // accept the proposal
                u_partner_list.add_partner(std::make_pair(v_rank, v));
                v_partner_list.add_partner(std::make_pair(u_rank, u));
            }
            
//std::cout << "u: " << u->get_id() << ' ' << vertex_level[u] << ' ' << proposal_index[u]<<'\n';
            //if (in_queue[u] == 1) { continue; }
            // add u to the free_list if it has residual capacity
            // and hasn't exhausted its preference list
            if (proposal_index[u]+1 < u_pref_list.size() and u->get_upper_quota() > u_partner_list.size() and in_queue[u] == 0) {
                // set the proposing index to the next vertex
                proposal_index[u] += 1;
                free_list.push(u);
                in_queue[u] = 1;
            } else if (proposal_index[u] == u_pref_list.size() and vertex_level[u] < max_level + 1) {
                // u can be promoted to a higher level
std::cout << "u: " << u->get_id() << ' ' << vertex_level[u] << ' ' << proposal_index[u]<<'\n';
                vertex_level[u] += 1;
                proposal_index[u] = 0;
                free_list.push(u);
                in_queue[u] = 1;
            }
        }
    }

    return true;
}
