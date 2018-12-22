#include "NProposingMatching.h"
#include "Vertex.h"
#include "Partner.h"
#include "PartnerList.h"
#include "Utils.h"
#include <stack>
#include <map>
#include <set>

NProposingMatching::NProposingMatching(const std::unique_ptr<BipartiteGraph>& G,
                                       bool A_proposing, int max_level)
    : MatchingAlgorithm(G, A_proposing), max_level(max_level)
{}

void NProposingMatching::add_matched_partners(VertexPtr u, VertexPtr v,
                                              int u_level,
                                              const PrefListBounds& u_pref_list_bounds,
                                              const PreferenceList& v_pref_list)
{
    // v is the first vertex on u's current preference list
    // invariant for non proposing vertices: rank = index + 1
    // v is always at level 0
    M_[u].add_partner(v, (RankType) u_pref_list_bounds.begin + 1, 0);

    M_[v].add_partner(u, compute_rank(u, v_pref_list, u_level), u_level);
}

// FIXME: a vertex that is evicted makes two proposals before its pref list bounds get updated.
bool NProposingMatching::compute_matching() {
    std::stack<VertexPtr> free_list;
    std::map<VertexPtr, int> vertex_level;
    std::map<VertexPtr, PrefListBounds> pref_list_bounds;
    const std::unique_ptr<BipartiteGraph>& G = get_graph();

    // choose the paritions from which the vertices will propose
    const auto& proposing_partition = is_A_proposing() ? G->get_A_partition()
                                                       : G->get_B_partition();

    // set the level of every vertex in the proposing partition to 0
    // mark all proposing vertices free (by pushing into the free_list)
    // and vertices from the opposite partition implicitly free
    for (auto& it : proposing_partition) {
        auto v = it.second;

        free_list.push(v);
        vertex_level[v] = 0;
        pref_list_bounds[v] = PrefListBounds(0, v->get_preference_list().size());
    }

    // there is at least one vertex in the free list
    while (not free_list.empty()) {
        // first vertex in free list
        auto u = free_list.top();
        auto& u_pref_list = u->get_preference_list();
        free_list.pop(); // remove u from free_list

        // if u^l hasn't exhausted its preference list
        if (! pref_list_bounds[u].is_exhausted()) {
            // highest ranked vertex to whom u not yet proposed
            auto v_it = u_pref_list.get(pref_list_bounds[u].begin);
            auto v = v_it.vertex;
            auto v_pref_list = v->get_preference_list();

            // |M[v]| = upper_quota(v)
            if (number_of_partners(M_, v) == v->get_upper_quota()) {
                auto v_worst_partner = M_[v].get_least_preferred();
                auto u_in_v_pref_list = v_pref_list.find(u);
                auto possible_partner = Partner(u, u_in_v_pref_list->rank, vertex_level[u]);

                if (v_worst_partner < possible_partner) {
                    // add u and v to the matching
                    add_matched_partners(u, v, vertex_level.at(u), pref_list_bounds[u], v_pref_list);

                    // remove v, v_worst_partner from M
                    M_[v].remove_least_preferred();
                    M_[v_worst_partner.vertex].remove(v);

                    // add v_worst_partner to free_list
                    free_list.push(v_worst_partner.vertex);
                }
            } else {
                // add u and v to the matching
                add_matched_partners(u, v, vertex_level.at(u), pref_list_bounds[u], v_pref_list);
            }

            // add u to the free_list if it has residual capacity
            // and hasn't exhausted its preference list
            if (u->get_upper_quota() > number_of_partners(M_, u)) {
                pref_list_bounds[u].begin += 1;
                free_list.push(u);
            }
        } else if (vertex_level[u] < max_level) {
            vertex_level[u] += 1;
            pref_list_bounds[u].begin = 0;
            free_list.push(u);
        }
    }

    return true;
}
