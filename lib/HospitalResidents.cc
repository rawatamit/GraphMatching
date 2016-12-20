#include "HospitalResidents.h"
#include <algorithm>

HospitalResidents::HospitalResidents(std::unique_ptr<BipartiteGraph> G)
    : MatchingAlgorithm(G)
{}

StableMatchingCapacity::~StableMatchingCapacity()
{}

namespace {
// comparison function
bool partner_cmp(Vertex::PartnerListType::value_type const& x,
                 Vertex::PartnerListType::value_type const& y)
{
    // compare w.r.t. ranks
    return x.first < y.first;
}
} // end of anonymous namespace

void HospitalResidents::compute_matching() {
    std::stack<BipartiteGraph::VertexType> free_list;

    // mark all men (by pushing into the free_list) and women (implicitly) free
    for (auto m : G->get_A_partition()) {
        free_list.push(m);
    }

    while (not free_list.empty()) {
        auto m = free_list.top();
        auto& m_pref_list = m->get_preference_list();
        free_list.pop(); // remove m from free_list

        // if the preferences of m have not been exhausted
        if (not m_pref_list.empty()) {
            // highest ranked woman to whom m has not yet proposed
            auto w = m_pref_list.get_proposal_partner();
            auto& w_pref_list = w->get_preference_list();
//            auto max_capacity = w->get_max_capacity();
            Vertex::PartnerListType& wpartners = w->get_partners();

            if (wpartners.size() < max_capacity) {
                w->add_partner(m);
            } else {
                auto maxe = std::max_element(wpartners.begin(), wpartners.end(), partner_cmp);

                // compare the element with the max rank to m
                if (maxe->first > w->get_rank(m)) {
                    free_list.push(maxe->second); // add to the free list
                    wpartners.erase(maxe->first); // remove the highest ranked vertex
                    w->add_partner(m);
                } else {
                    // w is unable to be matched to m
                    // add m to the free list again
                    free_list.push(m);
                }
            }
        }
    }
}
