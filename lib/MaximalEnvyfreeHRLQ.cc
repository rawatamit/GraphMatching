#include "MaximalEnvyfreeHRLQ.h"
#include "YokoiEnvyfreeHRLQ.h"
#include "StableMarriage.h"
#include "Vertex.h"
#include "TDefs.h"
#include "Utils.h"

MaximalEnvyfreeHRLQ::MaximalEnvyfreeHRLQ(std::shared_ptr<BipartiteGraph> G, bool A_proposing)
    : MatchingAlgorithm(std::move(G), A_proposing)
{}

void MaximalEnvyfreeHRLQ::matching_union(std::shared_ptr<MatchedPairListType> M1,
                                         std::shared_ptr<MatchedPairListType> M2) {
    for (auto& it : *M2) {
        auto& u = it.first;

        for (auto& v : it.second) {
            add_partner(M1, u, v, 0);
        }
    }
}

std::shared_ptr<MatchingAlgorithm::MatchedPairListType> MaximalEnvyfreeHRLQ::compute_matching() {
    YokoiEnvyfreeHRLQ y (get_graph(), is_A_proposing());
    auto M = y.compute_matching();

    // if there is no envy free matching in the graph
    // we cannot compute a maximal envy free matching
    if (M->empty()) {
        return std::make_shared<MatchedPairListType>();
    } else {
        auto G1 = augment_graph(M);

        // find a stable matching in the augmented graph
        StableMarriage sm(G1, is_A_proposing());
        auto M1 = map_inverse(sm.compute_matching());
        matching_union(M1, M);
        return M1;
    }
}

std::shared_ptr<BipartiteGraph> MaximalEnvyfreeHRLQ::augment_graph(std::shared_ptr<MatchedPairListType> M) const {
    BipartiteGraph::ContainerType A, B;
    std::shared_ptr<BipartiteGraph> G = get_graph();

    // add all vertices from partition B / hospitals
    for (auto& it : G->get_B_partition()) {
        auto v = it.second;

        // create a new vertex with quota (0, u_h - |M(h)|)
        auto u_id = v->get_id();
        auto nmatched = number_of_partners(M, v);
        auto u = std::make_shared<Vertex>(u_id, 0, v->get_upper_quota() - nmatched);

        auto& v_pref_list = v->get_preference_list();
        auto& u_pref_list = u->get_preference_list();

        // create preference list for h
        for (auto& i : v_pref_list) {
            auto r_old = i.vertex;
            auto& r_pref_list = r_old->get_preference_list();
            auto rit = M->find(r_old);

            // r_old is unmatched
            if (rit == M->end()) {
                auto r_id = r_old->get_id();

                // if not in the A partition
                // create a new resident and add
                if (A.find(r_id) == A.end()) {
                    auto r = std::make_shared<Vertex>(r_id,
                                r_old->get_lower_quota(), r_old->get_upper_quota());
                    A.emplace(r_id, r);
                }

                // add r to u's pref list
                u_pref_list.emplace_back(A.find(r_id)->second);
            } else {
                auto Mr = rit->second.get_least_preferred().vertex;// rit->second.get_vertex(rit->second.get_least_preferred());

                // v rank on r_old's preference list
                auto v_rank = r_pref_list.find(v)->rank;

                // M[r] rank on r_old's preference list
                auto Mr_rank = r_pref_list.find(Mr)->rank;

                // if r_old is matched, but r_old prefers M[r_old] over h
                // do not add r_old to the preference list of h
                // but continue going over the preference list of h
                if (v_rank >= Mr_rank) { continue; }

                // if r_old is matched, and r_old prefers h over M[r_old]
                // do not go over the preference list of h starting at r_old
                if (v_rank < Mr_rank) { break; }
            }
        }
        
        // add this vertex to partition B if its preference list is non empty
        if (u_pref_list.size() > 0 and u->get_upper_quota() > 0) {
            B.emplace(u_id, u);
        }
    }

    // settle preferences for the residents in the new graph
    auto& A_old = G->get_A_partition();

    for (auto& it : A) {
        auto r = it.second;
        auto& r_old = A_old.at(r->get_id());
        auto& r_pref_list = r->get_preference_list();
        auto& old_pref_list = r_old->get_preference_list();

        for (auto& i : old_pref_list) {
            auto h_old = i.vertex;

            // add this vertex to pref list only if it is in B
            if (B.find(h_old->get_id()) != B.end()) {
                auto h_pref_list = B.find(h_old->get_id())->second->get_preference_list();
                
                if (h_pref_list.find(r) != h_pref_list.cend()) {
                    r_pref_list.emplace_back(B.at(h_old->get_id()));
                }
            }
        }
    }

    return std::make_shared<BipartiteGraph>(A, B);
}
