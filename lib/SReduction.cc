#include "SReduction.h"
#include "Vertex.h"
#include "TDefs.h"
#include "Utils.h"
#include <map>

std::unique_ptr<BipartiteGraph> augment_graph(
        const std::unique_ptr<BipartiteGraph>& G, int s)
{
    BipartiteGraph::ContainerType A, B;

    // first add vertices from partition B to vmap
    for (auto it : G->get_B_partition()) {
        // vertex in partition B and their capacities remain unchanged
        auto& v = it.second;
        auto u_id = v->get_id();
        auto u = std::make_shared<Vertex>(u_id,
                        v->get_lower_quota(), v->get_upper_quota());

        // add this vertex to partition B
        B.emplace(u_id, u);
    }

    // then go about building the preference list of vertices in partition A
    // and adding them to vmap
    for (auto it : G->get_A_partition()) {
        auto& v = it.second;
        auto& v_id = v->get_id();
        auto& v_pref_list = v->get_preference_list();

        // a vertex m in partition A has (s-1) dummy hospitals in G_,
        // d^0_m, ..., d^(s-2)_m
        for (int k = 0; k < s-1; ++k) {
            const auto& dummy_id = get_dummy_id(v_id, k);
            auto dummy = std::make_shared<Vertex>(dummy_id, v_id, 0, 1, true);

            // add this dummy to partition B
            B.emplace(dummy_id, dummy);
        }

        // a vertex m in partition A have s copies in G_,
        // m_0, ..., m_(s-1)
        for (int k = 0; k < s; ++k) {
            const auto& u_id = get_vertex_id(v_id, k);
            auto u = std::make_shared<Vertex>(u_id, v_id,
                        v->get_lower_quota(), v->get_upper_quota(), false);
            auto& u_pref_list = u->get_preference_list();

            // add this vertex to partition A
            A.emplace(u_id, u);

            // for a level-0 vertex, its preference is the original preference
            // list followed by d^0_m
            if (k == 0) {
                for (auto i = v_pref_list.begin(), e = v_pref_list.end();
                     i != e; ++i)
                {
                    const auto& neighbour = v_pref_list.get_vertex(i);
                    u_pref_list.emplace_back(B.at(neighbour->get_id()));
                }

                const auto& dummy_0 = get_dummy_id(v_id, 0);
                u_pref_list.emplace_back(B.at(dummy_0));
            } else if (k == (s-1)) {
                // for a level-(s-1) vertex, its preference is d^(s-2)_m followed
                // by the original preference list
                const auto& dummy_s_2 = get_dummy_id(v_id, s-2);
                u_pref_list.emplace_back(B.at(dummy_s_2));

                for (auto i = v_pref_list.begin(), e = v_pref_list.end();
                     i != e; ++i)
                {
                    const auto& neighbour = v_pref_list.get_vertex(i);
                    u_pref_list.emplace_back(B.at(neighbour->get_id()));
                }
            } else { // 1 <= k <= s-2
                const auto& dummy_k_1 = get_dummy_id(v_id, k-1);
                u_pref_list.emplace_back(B.at(dummy_k_1));

                for (auto i = v_pref_list.begin(), e = v_pref_list.end();
                     i != e; ++i)
                {
                    const auto& neighbour = v_pref_list.get_vertex(i);
                    u_pref_list.emplace_back(B.at(neighbour->get_id()));
                }

                const auto& dummy_k = get_dummy_id(v_id, k);
                u_pref_list.emplace_back(B.at(dummy_k));
            }
        }
    }

    // create the preferences for the vertices in partition B
    for (auto it : G->get_B_partition()) {
        auto v = it.second;
        auto& v_pref_list = v->get_preference_list();

        // create the preference list for the corresponding vertex in G_
        // the preference list of this vertex is
        // all the level-(s-1) vertices, followed by level-(s-2) residents
        // and so on, in the same order as the original preference list
        auto vertex = B.at(v->get_id());
        auto& pref_list = vertex->get_preference_list();

        for (int k = (s-1); k >= 0; --k) {
           for (auto i = v_pref_list.begin(), e = v_pref_list.end();
                i != e; ++i)
           {
               const auto& u = v_pref_list.get_vertex(i);
               const auto& u_k_id = get_vertex_id(u->get_id(), k);
               pref_list.emplace_back(A.at(u_k_id));
           }
        }
    }

    // create the preferences for the dummies
    for (auto it : B) {
        auto v = it.second;

        // the preference list for a dummy d^i_r is
        // d^i_r : r^i, r^(i+1)
        if (v->is_dummy()) {
            const auto& dummy_id = v->get_id();
            int dummy_level = get_dummy_level(dummy_id);
            auto& pref_list = v->get_preference_list();
            const auto& r = v->get_cloned_for_id();

            pref_list.emplace_back(A.at(get_vertex_id(r, dummy_level)));
            pref_list.emplace_back(A.at(get_vertex_id(r, dummy_level+1)));
        }
    }

    return std::make_unique<BipartiteGraph>(A, B);
}

