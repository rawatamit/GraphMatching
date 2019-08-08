#include "HHeuristicHRLQ.h"
#include "StableMarriage.h"
#include "Vertex.h"
#include "PartnerList.h"
#include "TDefs.h"
#include "Utils.h"
#include <stack>
#include <cassert>

HHeuristicHRLQ::HHeuristicHRLQ(std::shared_ptr<BipartiteGraph> G, bool A_proposing)
    : MatchingAlgorithm(std::move(G), A_proposing)
{}

std::shared_ptr<MatchingAlgorithm::MatchedPairListType> HHeuristicHRLQ::compute_matching() {
    std::shared_ptr<BipartiteGraph> G = get_graph();

    // how many s values do we need to try
    int s_end = 10;//std::min(10u, G->get_B_partition().size());

    // compute a hospital proposing stable matching on the original graph
    StableMarriage sm(G, false);
    auto M = sm.compute_matching();
    std::map<IdType, PartnerList::SizeType > def;
    bool feasible = true; // assume the matching was feasible

    // compute deficiency for each lower-quota hospital
    for (auto& it : G->get_B_partition()) {
        auto v = it.second;
        auto& v_id = v->get_id();

        if (v->get_lower_quota() > 0) {
            PartnerList::SizeType nmatched = number_of_partners(M, v);
            PartnerList::SizeType deficiency = v->get_lower_quota() - nmatched;
            def.emplace(v_id, deficiency);

            // at least one lower quota hospital has deficiency > 0
            // therefore M_s is not feasible in G
            if (deficiency > 0) {
                feasible = false;
            }
        }
    }

    // we have a feasible matching, return it
    if (feasible) {
        return M;
    } else {
        // for (int s = 2; s < send; s += 2) {
        int s = s_end;
        G2_ = augment_phase2(s);
        compute_phase2_matching(M, def, s);
        //if (not G2_->has_augmenting_path(M_tmp_)) {
        const auto& M_inv = map_inverse(M_tmp_);
        return is_feasible(G, M_inv) ? M_inv : std::make_shared<MatchedPairListType>();
    }
}

bool HHeuristicHRLQ::compute_phase2_matching(std::shared_ptr<MatchedPairListType> M,
                                             std::map<IdType, PartnerList::SizeType >& def,
                                             int s)
{
    // free list with hospitals having deficiency > 0
    std::stack<VertexPtr> free_list;

    // map to keep track of which hospital has a copy in the queue
    std::map<IdType, int> in_queue;

    // partitions of the graph G2_
    const auto& A = G2_->get_A_partition();
    const auto& B = G2_->get_B_partition();

    // original graph G
    std::shared_ptr<BipartiteGraph> G = get_graph();

    // find the vertex given a bipartite graph and the information whether it
    // lies in the first partition or not
    auto find_vertex = [] (std::shared_ptr<BipartiteGraph> G,
                          const IdType& u, bool is_A_partition=true)
    {
        return is_A_partition ? G->get_A_partition().find(u)->second :
                                G->get_B_partition().find(u)->second;
    };

    // map the edges in M (matching in G) to M_tmp_ (matching in G2_)
    for (auto& it : A) {
        auto u = it.second;
        auto& pu_id = u->get_cloned_for_id();
        auto pu_it = M->find(find_vertex(G, pu_id));

        if (pu_it != M->end()) {
            auto& partner_list = pu_it->second;
            auto& u_pref_list = u->get_preference_list();

            for (auto& pit : partner_list) {
                // change every (r, h) edge to (r, h^0)
                auto h = pit.vertex;

                // get vertex h^0 from B
                const auto& h0_id = get_vertex_id(h->get_id(), 0);
                auto& h0 = B.at(h0_id);

                // add h^0 to the matched partner list of u
                auto h0_it = u_pref_list.find(h0);
                add_partner(M_tmp_, u, h0, h0_it->rank, 0);

                // add u to the matched partner list of h^0
                auto& h0_pref_list = h0->get_preference_list();
                auto u_it = h0_pref_list.find(u);
                add_partner(M_tmp_, h0, u, u_it->rank, 0);
            }
        }
    }

    std::map<IdType, PartnerList::SizeType > nmatched_count;
    for (auto& it : G->get_B_partition()) {
        auto v = it.second;
        in_queue[v->get_id()] = 0;

        if (M->find(v) != M->end()) {
            nmatched_count[v->get_id()] = M->at(v).size();
        }
    }

    std::map<IdType, std::pair<PreferenceList::SizeType, PreferenceList::SizeType>> pref_list_bounds;
    // only add level-1 lower quota hospitals with deficiency > 0 to the queue
    for (auto& it : def) {
        auto& pv_id = it.first;

        // if this hospital has deficiency > 0 add it to the free_list
        // also mark that it is inside the free_list
        if (it.second > 0) {
            // get level-1 copy corresponding to this hospital
            const auto& v_id = get_vertex_id(pv_id, 1);
            auto& v = B.at(v_id);
            free_list.push(v);
            pref_list_bounds[v->get_id()] = std::make_pair<>(0, v->get_preference_list().size());
            in_queue[pv_id] = 1;
        }
    }

    while (not free_list.empty()) {
        auto u = free_list.top();
        auto& u_pref_list = u->get_preference_list();
        auto& u_partner_list = (*M_tmp_)[u];
        auto const& pu_id = u->get_cloned_for_id();
        free_list.pop(); // remove u from free_list
        in_queue[pu_id] = 0;

        // if the preferences of u have not been exhausted
        if (pref_list_bounds[u->get_id()].first < pref_list_bounds[u->get_id()].second) {
            // highest ranked vertex to whom u not yet proposed
            auto v_it = u_pref_list.at(pref_list_bounds[u->get_id()].first);
            auto v = v_it.vertex;

            // v's preference list and list of partners
            auto& v_pref_list = v->get_preference_list();
            auto& v_partner_list = (*M_tmp_)[v];

            // u's rank on v's preference list
            auto u_rank = v_pref_list.find(u)->rank;

            // v's rank on u's preference list
            auto v_rank = v_it.rank;

            if (v_partner_list.size() == v->get_upper_quota()) {
                // v's least preferred partner
                auto worst_partner = v_partner_list.get_least_preferred();

                // worst partners rank, and preference list
                auto uc = worst_partner.vertex;
                auto uc_rank = worst_partner.rank;
                auto& uc_partner_list = (*M_tmp_)[uc];

                // does v prefer u over its worst partner?
                if (u_rank < uc_rank) {
                    // remove uc from v's matched partner list
                    v_partner_list.remove_least_preferred();

                    // add (u, v) in the matching
                    v_partner_list.add_partner(u, u_rank, 0);
                    u_partner_list.add_partner(v, v_rank, 0);

                    // we added a partner to u, decrease its deficiency
                    // and bump the number of matched partners
                    def[pu_id] -= 1;
                    nmatched_count[pu_id] += 1;

                    // mark uc free
                    uc_partner_list.remove(v);

                    const auto& puc_id = uc->get_cloned_for_id();
                    int uc_level = get_vertex_level(uc->get_id()); // get level of uc
                    nmatched_count[puc_id] -= 1;

                    def[puc_id] += 1; // add 1 to deficiency
                    if (uc->get_lower_quota() > 0) {
                        // is any copy of uc in the queue
                        if (in_queue[puc_id] == 1) {
                            // do nothing
                        } else if (def[puc_id] > 0) {
                            // push uc to free list
                            free_list.push(uc);
                            in_queue[puc_id] = 1;
                        } else if (uc_level == 0 and uc->get_upper_quota() > nmatched_count[puc_id]) {
                            free_list.push(uc);
                            in_queue[puc_id] = 1;
                        }
                    } else {
                        assert(uc_level == 0 && "hospital is non lq but level is not 0");

                        if (in_queue[puc_id] == 0 and uc->get_upper_quota() > nmatched_count[puc_id]) {
                            free_list.push(uc);
                            in_queue[puc_id] = 1;
                        }
                    }
                } else {
                    // u was rejected by v
                }
            } else { // v has residual capacity
                // accept the proposal
                v_partner_list.add_partner(u, u_rank, 0);
                u_partner_list.add_partner(v, v_rank, 0);

                // we added a partner to u, decrease its deficiency
                nmatched_count[pu_id] += 1;
                nmatched_count[v->get_cloned_for_id()] += 1;
                def[pu_id] -= 1;
            }

            int u_level = get_vertex_level(u->get_id()); // get level of u
            if (u->get_lower_quota() > 0) {
                if (in_queue[pu_id] == 1) {
                    // do nothing
                } else if (def[pu_id] > 0) {
                    pref_list_bounds[u->get_id()].first += 1;
                    free_list.push(u);
                    in_queue[pu_id] = 1;
                } else if (u_level == 0 and u->get_upper_quota() > nmatched_count[pu_id]) {
                    pref_list_bounds[u->get_id()].first += 1;
                    free_list.push(u);
                    in_queue[pu_id] = 1;
                }
            } else {
                assert(u_level == 0 && "hospital is non lq but level is not 0");

                if (in_queue[pu_id] == 0 and u->get_upper_quota() > nmatched_count[pu_id]) {
                    pref_list_bounds[u->get_id()].first += 1;
                    free_list.push(u);
                    in_queue[pu_id] = 1;
                }
            }
        } else {
            // get level of u
            int u_level = get_vertex_level(u->get_id());

            // if u has deficiency > 0 and its level < s
            if (u->get_lower_quota() > 0 and def[pu_id] > 0 and u_level < s) {
                const auto& nu_id = get_vertex_id(u->get_cloned_for_id(), u_level + 1);
                auto& nu = B.at(nu_id);
                pref_list_bounds[nu_id] = std::make_pair<>(0, nu->get_preference_list().size());
                free_list.push(nu);
            }
        }
    }

    return true;
}

std::shared_ptr<BipartiteGraph> HHeuristicHRLQ::augment_phase2(int s) {
    BipartiteGraph::ContainerType A, B;
    std::shared_ptr<BipartiteGraph> G = get_graph();
    std::map<IdType, std::vector<IdType>> lq_map; // lq hospitals for a resident

    // first add vertices from partition A
    for (auto& it : G->get_A_partition()) {
        // vertex in partition A and their capacities remain unchanged
        auto v = it.second;
        auto& v_id = v->get_id();
        auto& v_pref_list = v->get_preference_list();
        auto u = std::make_shared<Vertex>(v_id,
                        v->get_lower_quota(), v->get_upper_quota());

        for (auto& i : v_pref_list) {
            auto h = i.vertex;

            // only add lower quota vertices
            if (h->get_lower_quota() > 0) {
                lq_map[v_id].emplace_back(h->get_id());
            }
        }

        // add this vertex to partition A
        A.emplace(v_id, u);
    }

    // add vertices to partition B and build their preference list
    for (auto& it : G->get_B_partition()) {
        auto v = it.second;
        auto& v_id = v->get_id();
        auto& v_pref_list = v->get_preference_list();

        // a lower quota hospital in partition B have s copies in G_,
        if (v->get_lower_quota() > 0) {
            for (int k = 1; k <= s; ++k) {
                const auto& u_id = get_vertex_id(v_id, k);
                auto u = std::make_shared<Vertex>(u_id, v_id,
                            v->get_lower_quota(), v->get_upper_quota(), false);
                auto& u_pref_list = u->get_preference_list();

                // add this vertex to partition B
                B.emplace(u_id, u);

                // for a level-i (i > 0) hospital, its preference is the same
                // as the original preference list
                for (auto& i : v_pref_list) {
                    auto neighbour = i.vertex;
                    u_pref_list.emplace_back(A.at(neighbour->get_id()));
                }
            }
        }

        // every hospital has a level-0 copy in G_
        const auto& u_id = get_vertex_id(v_id, 0);
        auto u = std::make_shared<Vertex>(u_id, v_id,
                          v->get_lower_quota(), v->get_upper_quota(), false);
        auto& u_pref_list = u->get_preference_list();

        // for a level-0 hospital, its preference is the same
        // as the original preference list
        for (auto& i : v_pref_list) {
            auto neighbour = i.vertex;
            u_pref_list.emplace_back(A.at(neighbour->get_id()));
        }

        // add this vertex to partition B
        B.emplace(u_id, u);
    }

    // create the preferences for the vertices in partition A
    for (auto& it : G->get_A_partition()) {
        auto v = it.second;
        auto& v_id = v->get_id();
        auto& v_pref_list = v->get_preference_list();

        // create the preference list for the corresponding vertex in G_
        auto v_new = A.at(v_id);
        auto& pref_list = v_new->get_preference_list();

        // the preference list of this vertex is all the
        // level-(s-1) lq hospitals, followed by level-(s-2) lq hospitals
        // in the same order as the original preference list
        for (int k = s; k > 0; --k) {
            for (auto& lq_id : lq_map[v_id]) {
                const auto& u_k_id = get_vertex_id(lq_id, k);
                pref_list.emplace_back(B.at(u_k_id));
            }
        }

        // now we add all the hospitals in the original preference list in the
        // same order as in G
        for (auto& i : v_pref_list) {
            auto u = i.vertex;
            const auto& u_0_id = get_vertex_id(u->get_id(), 0);
            pref_list.emplace_back(B.at(u_0_id));
        }
    }

    return std::make_shared<BipartiteGraph>(A, B);
}
