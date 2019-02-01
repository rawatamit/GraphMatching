#include "YokoiEnvyfreeHRLQ.h"
#include "StableMarriage.h"
#include "Popular.h"
#include "Vertex.h"
#include "PartnerList.h"
#include "TDefs.h"
#include "Utils.h"
#include <stack>
#include <algorithm>
#include <cassert>

YokoiEnvyfreeHRLQ::YokoiEnvyfreeHRLQ(std::shared_ptr<BipartiteGraph> G, bool A_proposing)
    : MatchingAlgorithm(std::move(G), A_proposing)
{}

std::shared_ptr<MatchingAlgorithm::MatchedPairListType> YokoiEnvyfreeHRLQ::compute_matching() {
    std::shared_ptr<BipartiteGraph> G1 = augment_graph();

    // find a stable matching
    StableMarriage sm(G1, is_A_proposing());

    // compute stable matching
    auto M = sm.compute_matching();

    // for every vertex h in B, check if exactly
    // l_h partners are matched
    for (auto& it : G1->get_B_partition()) {
        auto v = it.second;

        // at least one hospital has |M_h| != l_h
        // therefore M_s is not feasible in G1
        if (number_of_partners(M, v) != v->get_upper_quota()) {
            return std::make_shared<MatchedPairListType>();
        }
    }

    // we have a feasible matching, return it
    return map_inverse(M);
}

std::shared_ptr<BipartiteGraph> YokoiEnvyfreeHRLQ::augment_graph() const {
    BipartiteGraph::ContainerType A, B;
    std::shared_ptr<BipartiteGraph> G = get_graph();

    // add all vertices from partition B / hospitals
    for (auto& it : G->get_B_partition()) {
        auto v = it.second;

        // only keep vertices in B with > 0 lower quota
        if (v->get_lower_quota() > 0) {
            // create a new vertex with quota (0, l_h)
            auto u_id = v->get_id();
            auto u = std::make_shared<Vertex>(u_id, 0, v->get_lower_quota());

            // add this vertex to partition B
            B.emplace(u_id, u);

            // preference list of this vertex is same as in G
            auto& u_pref_list = u->get_preference_list();

            // copy the preference list as it is
            for (auto& i : v->get_preference_list()) {
                auto r_old = i.vertex;
                auto r_id = r_old->get_id();

                // check if this vertex already exists in A
                // if yes, then use it otherwise create a new one
                auto r_it_A = A.find(r_id);

                if (r_it_A != A.end()) {
                    u_pref_list.emplace_back(r_it_A->second);
                    // assert that this has the same lower and upper quotas
                    assert(r_it_A->second->get_lower_quota() == r_old->get_lower_quota());
                    assert(r_it_A->second->get_upper_quota() == r_old->get_upper_quota());
                } else {
                    auto r = std::make_shared<Vertex>(r_id, r_old->get_lower_quota(), r_old->get_upper_quota());
                    // add the neighbouring vertex to A as well
                    A.emplace(r_id, r);
                    u_pref_list.emplace_back(r);
                }
            }
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

            // add this vertex to pref list only if it has a positive lower quota
            if (h_old->get_lower_quota() > 0) {
                r_pref_list.emplace_back(B.at(h_old->get_id()));
            }
        }
    }

    return std::make_shared<BipartiteGraph>(A, B);
}
