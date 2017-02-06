#include "HHeuristicHRLQ.h"
#include "StableMarriage.h"
#include "Popular.h"
#include "Vertex.h"
#include "PartnerList.h"
#include "TDefs.h"
#include "Utils.h"
#include <stack>
#include <queue>
#include <algorithm>
#include <cassert>

#include <iostream>

HHeuristicHRLQ::HHeuristicHRLQ(const std::unique_ptr<BipartiteGraph>& G, bool A_proposing)
    : MatchingAlgorithm(G)
{}

HHeuristicHRLQ::~HHeuristicHRLQ()
{}

bool HHeuristicHRLQ::compute_matching() {
    const std::unique_ptr<BipartiteGraph>& G = get_graph();
    StableMarriage sm(G, false);

    // how many s values do we need to try
    int send = std::min(10ul, G->get_B_partition().size());

    // compute a hospital proposing stable matching on the original graph
    if (sm.compute_matching()) {
        // compute deficiency for each lower-quota hospital
        std::map<IdType, int> def;
        auto& M = sm.get_matched_pairs();
        bool feasible = true; // assume the matching was feasible
 //print_matching(G, M, std::cerr);

    int sum_def = 0;
        for (auto it : G->get_B_partition()) {
            auto v = it.second;
            auto& v_id = v->get_id();

            if (v->get_lower_quota() > 0) {
                auto it = M.find(v);
                int nmatched = it != M.end() ? it->second.size() : 0;
                int deficiency = v->get_lower_quota() - nmatched;
                def.emplace(v_id, deficiency);

                // at least one lower quota hospital has deficiency > 0
                // therefore M_s is not feasible in G
                if (deficiency > 0) {
    sum_def += deficiency;
                    feasible = false;
                }
            }
        }

std::cerr << sum_def << '\n';
std::cerr << sum_def << ' ' << feasible << '\n';
        // we have a feasible matching, return it
        if (feasible) {
            M_ = M;
            return true;
        } else {
            // for (int s = 2; s < send; s += 2) {
            int s = send;
    // print_matching(G1_, pamc.get_matched_pairs(), std::cerr);
                G2_ = augment_phase2(M, s);
                compute_phase2_matching(M, def, s);
                //if (not G2_->has_augmenting_path(M_tmp_)) {
                const auto& M_inv = map_inverse(M_tmp_);
                return is_feasible(G, M_inv);
                    // return true;
                //} else {
                    //M_tmp_.clear();
                //}
            // }
        }
    }

    return false;
}

MatchedPairListType& HHeuristicHRLQ::get_matched_pairs() {
    return M_;
}

bool HHeuristicHRLQ::compute_phase2_matching(const MatchedPairListType& M,
                                             std::map<IdType, int>& def,
                                             int s)
{
    // free list with hospitals having deficiency > 0
    //std::stack<VertexPtr> free_list;
    std::queue<VertexPtr> free_list;

    // map to keep track of which hospital has a copy in the queue
    std::map<IdType, int> in_queue;

    // partitions of the graph G2_
    const auto& A = G2_->get_A_partition();
    const auto& B = G2_->get_B_partition();

    // original graph G
    const std::unique_ptr<BipartiteGraph>& G = get_graph();

    // find the vertex given a bipartite graph and the information whether it
    // lies in the first partition or not
    auto find_vertex = [] (const std::unique_ptr<BipartiteGraph>& G,
                          const IdType& u, bool is_A_partition=true)
    {
        return is_A_partition ? G->get_A_partition().find(u)->second :
                                G->get_B_partition().find(u)->second;
    };

    // map the edges in M (matching in G) to M_tmp_ (matching in G2_)
    for (auto it : A) {
        auto u = it.second;
        auto& pu_id = u->get_cloned_for_id();
        auto pu_it = M.find(find_vertex(G, pu_id));

        if (pu_it != M.end()) {
            const auto& partner_list = pu_it->second;
            auto& u_pref_list = u->get_preference_list();

            for (auto pit = partner_list.cbegin(), pie = partner_list.cend();
                 pit != pie; ++pit)
            {
                // change every (r, h) edge to (r, h^0)
                auto h = partner_list.get_vertex(pit);

                // get vertex h^0 from B
                const auto& h0_id = get_vertex_id(h->get_id(), 0);
                auto h0 = B.at(h0_id);

                // add h^0 to the matched partner list of u
                auto h0_it = u_pref_list.find(h0);
                M_tmp_[u].add_partner(std::make_pair(u_pref_list.get_rank(h0_it), h0));

                // add u to the matched partner list of h^0
                auto& h0_pref_list = h0->get_preference_list();
                auto u_it = h0_pref_list.find(u);
                M_tmp_[h0].add_partner(std::make_pair(h0_pref_list.get_rank(u_it), u));
            }
        }
    }

// print_matching(G2_, M_tmp_, std::cerr);

    std::map<IdType, int> nmatched_count;
    for (auto it : G->get_B_partition()) {
        auto v = it.second;
        in_queue[v->get_id()] = 0;
        if (M.find(v) != M.end()) {
            nmatched_count[v->get_id()] = M.at(v).size();
        }
    }

    // only add level-1 lower quota hospitals with deficiency > 0 to the queue
    for (auto it : def) {
        auto& pv_id = it.first;

        // if this hospital has deficiency > 0 add it to the free_list
        // also mark that it is inside the free_list
        if (it.second > 0) {
            // get level-1 copy corresponding to this hospital
            const auto& v_id = get_vertex_id(pv_id, 1);
            free_list.push(B.at(v_id));
            in_queue[pv_id] = 1;
        }
    }

    while (not free_list.empty()) {
        auto u = free_list.front();
        auto& u_pref_list = u->get_preference_list();
        auto& u_partner_list = M_tmp_[u];
        auto const& pu_id = u->get_cloned_for_id();
        free_list.pop(); // remove u from free_list
        in_queue[pu_id] = 0;

        // if the preferences of u have not been exhausted
        if (not u_pref_list.empty()) {
            // highest ranked vertex to whom u not yet proposed
            auto v = u_pref_list.get_vertex(u_pref_list.get_proposal_index());
    auto v_id = v->get_id();

 //std::cerr << u->get_id() << " proposes to " << v_id << '\n';
// if (u->get_id() == "h1^0") {std::cerr << u->get_id() << " is proposing " << v->get_id() << '\n'; }
            // v's preference list and list of partners
            auto& v_pref_list = v->get_preference_list();
            auto& v_partner_list = M_tmp_[v];

            // u's rank on v's preference list
            auto u_rank = v_pref_list.get_rank(v_pref_list.find(u));

            // v's rank on u's preference list
            auto v_rank = u_pref_list.get_rank(u_pref_list.get_proposal_index());

            if (v_partner_list.size() == v->get_upper_quota()) {
                // v's least preferred partner
                auto worst_partner = v_partner_list.get_least_preferred();

                // worst partners rank, and preference list
                auto uc = v_partner_list.get_vertex(worst_partner);
      auto uc_id = uc->get_id(); //auto ik = M_[uc];
                auto uc_rank = v_partner_list.get_rank(worst_partner);
                auto& uc_pref_list = uc->get_preference_list();
                auto& uc_partner_list = M_tmp_[uc];

 //std::cerr << v_id << " worst partner " << uc_id << '\n';
                // does v prefer u over its worst partner?
                if (u_rank < uc_rank) {
                    // remove uc from v's matched partner list
                    v_partner_list.remove_least_preferred();

                    // add (u, v) in the matching
                    v_partner_list.add_partner(std::make_pair(u_rank, u));
                    u_partner_list.add_partner(std::make_pair(v_rank, v));

                    // we added a partner to u, decrease its deficiency
                    // and bump the number of matched partners
                    def[pu_id] -= 1;
                    nmatched_count[pu_id] += 1;

                    // mark uc free
                    // NOTE: this should be uc_partner_list.remove(v);
                    // change the preference list representation if required
                    uc_partner_list.remove(v);

                    // remove v from uc's preferences
                    //uc_pref_list.remove_first();

                    const auto& puc_id = uc->get_cloned_for_id();
                    int uc_level = get_vertex_level(uc->get_id()); // get level of uc
                    nmatched_count[puc_id] -= 1;

                    def[puc_id] += 1; // add 1 to deficiency
                    if (uc->get_lower_quota() > 0) {
                        //def[puc_id] += 1; // add 1 to deficiency

                        // is any copy of uc in the queue
                        if (in_queue[puc_id] == 1) {
                            // do nothing
                        } else if (def[puc_id] > 0) {
                            // push uc to free list
// std::cerr << "push " << uc->get_id() << '\n';
                //uc_pref_list.remove_first();
//std::cerr << __LINE__ << "incrementing proposal index for " << uc->get_id() << '\n';
                    // uc_pref_list.remove_first();
                            free_list.push(uc);
                            in_queue[puc_id] = 1;
                        } else if (uc_level == 0 and not uc_pref_list.empty()
                                and uc->get_upper_quota() > nmatched_count[puc_id]) {
 //std::cerr << "push " << uc->get_id() << '\n';
                //uc_pref_list.remove_first();
//std::cerr << __LINE__ <<  "incrementing proposal index for " << uc->get_id() << " size pref_list " << uc_pref_list.size()<< '\n';
                    // uc_pref_list.remove_first();
                            free_list.push(uc);
                            in_queue[uc_id] = 1;
                        }
                    } else {
                        assert(uc_level == 0 && "hospital is non lq but level is not 0");
                        if (in_queue[puc_id] == 0
                             and not uc_pref_list.empty()
                            and uc->get_upper_quota() > nmatched_count[puc_id]) {
//std::cerr << __LINE__ << "incrementing proposal index for " << uc->get_id() << '\n';
                    // uc_pref_list.remove_first();
                            free_list.push(uc);
                            in_queue[puc_id] = 1;
                        }
                    }
                } else {
                    // u was rejected by v
                }

                //v_pref_list.restrict_preferences(u);
            } else { // v has residual capacity
                // accept the proposal
                v_partner_list.add_partner(std::make_pair(u_rank, u));
                u_partner_list.add_partner(std::make_pair(v_rank, v));

                // we added a partner to u, decrease its deficiency
                nmatched_count[pu_id] += 1;
                nmatched_count[v->get_cloned_for_id()] += 1;
                def[pu_id] -= 1;
            }

                    // u_pref_list.remove_first();
            int u_level = get_vertex_level(u->get_id()); // get level of u
            if (u->get_lower_quota() > 0) {
                if (in_queue[pu_id] == 1) {
                    // do nothing
                } else if (def[pu_id] > 0) {
//std::cerr << __LINE__ << "incrementing proposal index for " << u->get_id() << '\n';
                    u_pref_list.remove_first();
                    free_list.push(u);
                    in_queue[pu_id] = 1;
                } else if (u_level == 0 and not u_pref_list.empty()
                        and u->get_upper_quota() > nmatched_count[pu_id]) {
//std::cerr << __LINE__ << "incrementing proposal index for " << u->get_id() << '\n';
                    u_pref_list.remove_first();
                    free_list.push(u);
                    in_queue[pu_id] = 1;
                }
            } else {
                assert(u_level == 0 && "hospital is non lq but level is not 0");
                if (in_queue[pu_id] == 0
                     and not u_pref_list.empty()
                    and u->get_upper_quota() > nmatched_count[pu_id]) {
//std::cerr << __LINE__ << "incrementing proposal index for " << u->get_id() << '\n';
                    u_pref_list.remove_first();
                    free_list.push(u);
                    in_queue[pu_id] = 1;
                }
            }

                    //int u_level = get_vertex_level(u->get_id()); // get level of uc
            //if (u_level == 0 and not u_pref_list.empty()
                //and u->get_upper_quota() > nmatched_count[pu_id]) {
                //u_pref_list.remove_first();
                //free_list.push(u);
            //}
// if (u->get_id() == "h1^0") {std::cerr << u->get_id() << " has def " << def[pu_id] << '\n'; }
            // add u to the free_list if it has deficiency > 0
            //if (def[pu_id] > 0) {
                // remove v from u's preferences
                //u_pref_list.remove_first();
// std::cerr << "push " << u->get_id() << '\n';
                //free_list.push(u);
            //}
        } else {
            // get level of u
            int u_level = get_vertex_level(u->get_id());

            // if u has deficiency > 0 and its level < s
            if (u->get_lower_quota() > 0 and def[pu_id] > 0 and u_level < s) {
                const auto& nu_id = get_vertex_id(u->get_cloned_for_id(), u_level + 1);
 //std::cerr << "push " << nu_id << '\n';
                free_list.push(B.at(nu_id));
            }
        }
// print_matching(G2_, M_tmp_, std::cerr); std::cerr << "ends here\n";
    }

    return true;
}

std::unique_ptr<BipartiteGraph> HHeuristicHRLQ::augment_phase2(const MatchedPairListType& M, int s) {
    BipartiteGraph::ContainerType A, B;
    const std::unique_ptr<BipartiteGraph>& G = get_graph();
    std::map<IdType, std::vector<IdType>> lq_map; // lq hospitals for a resident

    // first add vertices from partition A
    for (auto it : G->get_A_partition()) {
        // vertex in partition A and their capacities remain unchanged
        auto v = it.second;
        auto& v_id = v->get_id();
        auto& v_pref_list = v->get_preference_list();
        auto u = std::make_shared<Vertex>(v_id,
                        v->get_lower_quota(), v->get_upper_quota());

        for (auto i = v_pref_list.all_begin(), e = v_pref_list.all_end(); i != e; ++i) {
            auto h = v_pref_list.get_vertex(*i);

            // only add lower quota vertices
            if (h->get_lower_quota() > 0) {
                lq_map[v_id].emplace_back(h->get_id());
            }
        }

        // add this vertex to partition A
        A.emplace(v_id, u);
    }

    // add vertices to partition B and build their preference list
    for (auto it : G->get_B_partition()) {
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
                for (auto i = v_pref_list.all_begin(), e = v_pref_list.all_end();
                     i != e; ++i)
                {
                    auto neighbour = v_pref_list.get_vertex(*i);
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
        for (auto i = v_pref_list.all_begin(), e = v_pref_list.all_end(); i != e; ++i) {
            auto neighbour = v_pref_list.get_vertex(*i);
            u_pref_list.emplace_back(A.at(neighbour->get_id()));
        }

        // add this vertex to partition B
        B.emplace(u_id, u);
    }

    // create the preferences for the vertices in partition A
    for (auto it : G->get_A_partition()) {
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
            for (auto lq_id : lq_map[v_id]) {
                const auto& u_k_id = get_vertex_id(lq_id, k);
                pref_list.emplace_back(B.at(u_k_id));
            }
        }

        // now we add all the hospitals in the original preference list in the
        // same order as in G
        for (auto i = v_pref_list.all_begin(), e = v_pref_list.all_end(); i != e; ++i) {
            auto u = v_pref_list.get_vertex(*i);
            const auto& u_0_id = get_vertex_id(u->get_id(), 0);
            pref_list.emplace_back(B.at(u_0_id));
        }
    }

    return std::make_unique<BipartiteGraph>(A, B);
}

