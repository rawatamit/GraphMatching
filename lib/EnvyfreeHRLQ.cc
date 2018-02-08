#include "EnvyfreeHRLQ.h"
#include "StableMarriage.h"
#include "Popular.h"
#include "Vertex.h"
#include "PartnerList.h"
#include "TDefs.h"
#include "Utils.h"
#include <stack>
#include <algorithm>
#include <cassert>

EnvyfreeHRLQ::EnvyfreeHRLQ(const std::unique_ptr<BipartiteGraph>& G, bool A_proposing)
    : MatchingAlgorithm(G)
{}

EnvyfreeHRLQ::~EnvyfreeHRLQ()
{}

MatchedPairListType& EnvyfreeHRLQ::get_matched_pairs() {
    return M_;
}

bool EnvyfreeHRLQ::compute_matching() {
    const std::unique_ptr<BipartiteGraph>& G1 = augment_graph();

    // find a resident proposing stable matching
    StableMarriage sm(G1);
    bool feasible = true;

    // compute stable matching
    if (sm.compute_matching()) {
        auto& M = sm.get_matched_pairs();

        // for every vertex h in B, check if exactly
        // l_h partners are matched
        for (auto it : G1->get_B_partition()) {
            auto v = it.second;

            // find v in M
            auto vit = M.find(v);
            int nmatched = vit != M.end() ? vit->second.size() : 0;

            // at least one hospital has |M_h| != l_h
            // therefore M_s is not feasible in G1
            if (nmatched != v->get_upper_quota()) {
                feasible = false;
            }
        }

        // we have a feasible matching, return it
        if (feasible) {
            M_ = map_inverse(M);
            return true;
        }
    }

    return false;
}

std::unique_ptr<BipartiteGraph> EnvyfreeHRLQ::augment_graph() {
    BipartiteGraph::ContainerType A, B;
    const std::unique_ptr<BipartiteGraph>& G = get_graph();

    // add all vertices from partition B / hospitals
    for (auto it : G->get_B_partition()) {
        auto v = it.second;

        // only keep vertices in B with > 0 lower quota
        if (v->get_lower_quota() > 0) {
            // create a new vertex with quota (0, l_h)
            auto u_id = v->get_id();
            auto u = std::make_shared<Vertex>(u_id, 0, v->get_lower_quota());

            // add this vertex to partition B
            B.emplace(u_id, u);

            // preference list of this vertex is same as in G
            auto& v_pref_list = v->get_preference_list();
            auto& u_pref_list = u->get_preference_list();

            // copy the preference list as it is
            for (auto i = v_pref_list.all_begin(), e = v_pref_list.all_end();
                    i != e; ++i)
            {
                auto r_old = v_pref_list.get_vertex(*i);
                auto r_id = r_old->get_id();
                auto r = std::make_shared<Vertex>(r_id,
                            r_old->get_lower_quota(), r_old->get_upper_quota());

                // add the neighbouring vertex to A as well
                A.emplace(r_id, r);
                u_pref_list.emplace_back(r);
            }
        }
    }

    // settle preferences for the residents in the new graph
    auto& A_old = G->get_A_partition();

    for (auto it : A) {
        auto r = it.second;
        auto r_old = A_old.at(r->get_id());
        auto& r_pref_list = r->get_preference_list();
        auto& old_pref_list = r_old->get_preference_list();

        // copy the preferences as it is
        for (auto i = old_pref_list.all_begin(), e = old_pref_list.all_end();
             i != e; ++i)
        {
            auto h_old = old_pref_list.get_vertex(*i);

            // add this vertex to pref list only if it has a positive lower quota
            if (h_old->get_lower_quota() > 0) {
                r_pref_list.emplace_back(B.at(h_old->get_id()));
            }
        }
    }

    return std::make_unique<BipartiteGraph>(A, B);
}
