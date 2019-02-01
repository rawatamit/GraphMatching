#include "RHeuristicHRLQ.h"
#include "StableMarriage.h"
#include "Popular.h"
#include "Vertex.h"
#include "PartnerList.h"
#include "TDefs.h"
#include "Utils.h"
#include <algorithm>

RHeuristicHRLQ::RHeuristicHRLQ(std::shared_ptr<BipartiteGraph> G, bool A_proposing)
    : MatchingAlgorithm(std::move(G), A_proposing)
{}

std::shared_ptr<MatchingAlgorithm::MatchedPairListType> RHeuristicHRLQ::compute_matching() {
    G1_ = augment_phase1();
    PopularAmongMaxCard pamc(G1_);

    G2_ = augment_phase2(pamc.compute_matching());
    // find a resident proposing stable matching
    StableMarriage sm(G2_);

    auto M = map_inverse(sm.compute_matching());
    return is_feasible(get_graph(), M) ? M : std::make_shared<MatchedPairListType>();
}

std::shared_ptr<BipartiteGraph> RHeuristicHRLQ::augment_phase1() {
    BipartiteGraph::ContainerType A, B;
    const std::shared_ptr<BipartiteGraph>& G = get_graph();

    // add only those vertices from partition B which have lower quota > 0
    for (auto& it : G->get_B_partition()) {
        auto v = it.second;

        if (v->get_lower_quota() > 0) {
            auto u_id = v->get_id();
            auto u = std::make_shared<Vertex>(u_id, 0, v->get_lower_quota());

            // add this vertex to partition B
            B.emplace(u_id, u);

            // preference list of this vertex is same as in G
            auto& v_pref_list = v->get_preference_list();
            auto& u_pref_list = u->get_preference_list();

            for (auto& i : v_pref_list) {
                auto r_old = i.vertex;
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

    for (auto& it : A) {
        auto r = it.second;
        auto& r_old = A_old.at(r->get_id());
        auto& r_pref_list = r->get_preference_list();
        auto& old_pref_list = r_old->get_preference_list();

        for (auto& i : old_pref_list) {
            // only add vertices with lower quota > 0
            auto h_old = i.vertex;
            if (h_old->get_lower_quota() > 0) {
                r_pref_list.emplace_back(B.at(h_old->get_id()));
            }
        }
    }

    return std::make_unique<BipartiteGraph>(A, B);
}

std::shared_ptr<BipartiteGraph> RHeuristicHRLQ::augment_phase2(std::shared_ptr<MatchedPairListType> M) {
    /// the matching M passed to this function is a matching in the graph G1_
    /// and not in the original graph G, therefore trying to get the matched
    /// partners in M will not work directly
    /// to get a matched partner M[u] where u is a vertex in the graph G
    /// first get the VertexPtr associated with u in G1_, and then probe M with
    /// it, this will fetch the partners of u in G1_, to get the original
    /// vertices in G, get their pointers from G using their id

    BipartiteGraph::ContainerType A, B;
    std::shared_ptr<BipartiteGraph> G = get_graph();

    // find the vertex given a bipartite graph and the information whether it
    // lies in the first partition or not
    auto find_vertex = [] (std::shared_ptr<BipartiteGraph> G,
                          const IdType& u, bool is_A_partition=true)
    {
        return is_A_partition ? G->get_A_partition().find(u)->second :
                                G->get_B_partition().find(u)->second;
    };

    // add vertices from partition B
    for (auto& it : G->get_B_partition()) {
        // vertex in partition B with the capacities (0, uq)
        auto v = it.second;
        auto u_id = v->get_id();
        auto u = std::make_shared<Vertex>(u_id, 0, v->get_upper_quota());

        // add this vertex to partition B
        B.emplace(u_id, u);
    }

    // build the preference list of vertices in partition A
    for (auto& it : G->get_A_partition()) {
        auto v = it.second;
        auto& v_id = v->get_id();

        // create a resident with the same preferences in the new graph
        auto r = std::make_shared<Vertex>(v_id,
                          v->get_lower_quota(), v->get_upper_quota());
        // add this vertex to partition A
        A.emplace(v_id, r);

        auto v_G1 = find_vertex(G1_, v_id);
        const auto& M_v = M->find(v_G1);

        // if this vertex has a partner, create a new resident and a dummy
        // corresponding to it, v is the original resident in G and not r
        if (M_v != M->end()) {
            // create a new level-0 resident
            const auto& r0_id = get_vertex_id(v_id, 0);
            auto r0 = std::make_shared<Vertex>(r0_id, v_id,
                        v->get_lower_quota(), v->get_upper_quota(), false);

            // add this vertex to partition A
            A.emplace(r0_id, r0);

            // create a dummy in partition B
            const auto& dummy_id = get_dummy_id(v_id, 0);
            auto dummy = std::make_shared<Vertex>(dummy_id, v_id, 0, 1, true);

            // add the dummy to partition B
            B.emplace(dummy_id, dummy);

            // create the preference list for the dummy,
            // as we have enough information at this point
            // d^r : r^0, r
            auto& dummy_pref_list = dummy->get_preference_list();
            dummy_pref_list.emplace_back(r0);
            dummy_pref_list.emplace_back(r);

            // add this dummy at the beginning of the preference list of r
            r->get_preference_list().emplace_back(dummy);

            // preference list r^0 : M(r), d^r
            auto& v_partners = M_v->second;
            auto& r0_pref_list = r0->get_preference_list();
            auto partner = v_partners.cbegin()->vertex;
            r0_pref_list.emplace_back(B.at(partner->get_id()));
            r0_pref_list.emplace_back(dummy);
        }

        // for r, its preference is d^r followed by its original preference list
        // d^r has already been added to its preference list
        auto& v_pref_list = v->get_preference_list();
        auto& r_pref_list = r->get_preference_list();

        for (auto& i : v_pref_list) {
            // get the old neighbouring hospital
            auto h_old = i.vertex;
            auto& h_id = h_old->get_id();

            // add h to r's preference list and the partition B
            r_pref_list.emplace_back(B.at(h_id));
        }
    }

    // create the preferences for the vertices in partition B
    for (auto& it : G->get_B_partition()) {
        auto v = it.second;
        auto v_id = v->get_id();
        auto& v_pref_list = v->get_preference_list();

        // create the preference list for the corresponding vertex in G2_
        // the preference list of this vertex is
        // all the residents in the original preference list followed by M(v)
        auto vertex = B.at(v_id);
        auto& pref_list = vertex->get_preference_list();

        for (auto& i : v_pref_list) {
            auto u = i.vertex;
            pref_list.emplace_back(A.at(u->get_id()));
        }

        // if this hospital has partners, add their level-0 copy
        // at the end of its preference list
        // in phase 1, only the lower quota hospitals as in the original graph
        // G are present in the graph, and so only they can be matched
        if (v->get_lower_quota() > 0) {
            auto v_G1 = find_vertex(G1_, v_id, false);
            const auto& M_v = M->find(v_G1);

            if (M_v != M->end()) {
                auto& partner_list = M_v->second;
                // TODO: is this important?
                // partner_list.sort();

                for (auto& pit : partner_list) {
                    auto r = pit.vertex;
                    const auto& r0_id = get_vertex_id(r->get_id(), 0);
                    pref_list.emplace_back(A.at(r0_id));
                }
            }
        }
    }

    return std::make_unique<BipartiteGraph>(A, B);
}
