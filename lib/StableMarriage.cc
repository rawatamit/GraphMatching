#include "StableMarriage.h"
#include "Vertex.h"
#include "PartnerList.h"
#include <stack>
#include <map>

StableMarriage::StableMarriage(const std::unique_ptr<BipartiteGraph>& G,
                               bool A_proposing)
    : MatchingAlgorithm(G), A_proposing_(A_proposing)
{}

StableMarriage::~StableMarriage()
{}

// #include <iostream>
bool StableMarriage::compute_matching() {
    /// we do not assume that the proposal index starts at 0
    /// in the algorithm below, this is to cover the case when
    /// one would like to restart the algorithm at a particular place
    /// if you want to get a stable matching from scratch make sure
    /// that the graph provided is pristine
    std::stack<VertexPtr> free_list;
    std::map<VertexPtr, int> in_queue;
    const std::unique_ptr<BipartiteGraph>& G = get_graph();

    // choose the paritions from which the vertices will propose
    const auto& proposing_partition = A_proposing_ ? G->get_A_partition()
                                                   : G->get_B_partition();

    // mark all proposing vertices free (by pushing into the free_list)
    // and vertices from the opposite partition implicitly free
    for (auto it : proposing_partition) {
        free_list.push(it.second);
        in_queue[it.second] = 1;
    }

    while (not free_list.empty()) {
        auto u = free_list.top();
        auto& u_pref_list = u->get_preference_list();
        auto& u_partner_list = M_[u];
        free_list.pop(); // remove u from free_list
        in_queue[u] = 0;

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

// {std::cerr << "u: " << u->get_id() << ", v: " << v->get_id() << '\n';}
  //<< u_rank << ' ' << uc_rank << '\n'; std::cerr << v_partner_list << '\n';}
//if(v_partner_list.size() > v->get_upper_quota()) {std::cerr << v->get_id() << " > uq " << v->get_upper_quota();}
            if (v_partner_list.size() == v->get_upper_quota()) {
                // v's least preferred partner
                auto worst_partner = v_partner_list.get_least_preferred();

                // worst partners rank, and preference list
                auto uc = v_partner_list.get_vertex(worst_partner);
                auto uc_rank = v_partner_list.get_rank(worst_partner);
                // auto& uc_pref_list = uc->get_preference_list();
                auto& uc_partner_list = M_[uc];

// if(u->get_id()=="a10")
//{std::cerr << "v: " << v->get_id() << ", u: " << u->get_id() << ", uc: " << uc->get_id() << ' '
  //<< u_rank << ' ' << uc_rank << '\n'; std::cerr << v_partner_list << '\n';}

//auto u_id = u->get_id(), v_id = v->get_id(), uc_id = uc->get_id();

                // does v prefer u over its worst partner?
                if (u_rank < uc_rank) {
                    // remove uc from v's matched partner list
                    v_partner_list.remove_least_preferred();

                    // add (u, v) in the matching
                    u_partner_list.add_partner(std::make_pair(v_rank, v));
                    v_partner_list.add_partner(std::make_pair(u_rank, u));

                    // mark uc free
                    // NOTE: this should be uc_partner_list.remove(v);
                    // change the preference list representation if required
                    // uc_partner_list.remove_least_preferred();
                    uc_partner_list.remove(v);

                    // remove v from uc's preferences
// auto pbefore = uc_pref_list.get_proposal_index();
                    //uc_pref_list.remove_first();
// auto pafter = uc_pref_list.get_proposal_index();
// if (pbefore == pafter) { std::cerr << "push uc: " << uc->get_id() << ' ' << pbefore << '\n'; }

                    // push uc to free list
        if (in_queue[uc] == 0){
                    free_list.push(uc);
                    in_queue[uc] = 1;
                    }

                } //else { u_pref_list.remove_first(); }
            } else {
                // accept the proposal
                u_partner_list.add_partner(std::make_pair(v_rank, v));
                v_partner_list.add_partner(std::make_pair(u_rank, u));
            }
u_pref_list.remove_first();
            // add u to the free_list if it has residual capacity
            if (u->get_upper_quota() > u_partner_list.size()) {
                // remove v from u's preferences
// auto pbefore = u_pref_list.get_proposal_index();
                //u_pref_list.remove_first();
// auto pafter = u_pref_list.get_proposal_index();
// if (pbefore == pafter) { std::cerr << "push u: " << u->get_id() << ' ' << pbefore << '\n'; }
        if (in_queue[u] == 0){
                free_list.push(u);
                in_queue[u] = 1;
}
            }
        }
    }

    return true;
}

