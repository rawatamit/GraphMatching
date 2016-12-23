#include "PopularMatching.h"
#include "SReduction.h"
#include "StableMarriage.h"
#include "Utils.h"
#include <stack>

PopularMatching::PopularMatching(const std::unique_ptr<BipartiteGraph>& G)
    : MatchingAlgorithm(G)
{}

PopularMatching::~PopularMatching()
{}

void PopularMatching::compute_matching() {
    const std::unique_ptr<BipartiteGraph>& G = get_graph();
    G_2_ = augment_graph(G, 2);
    StableMarriage sm(G_2_);
    sm.compute_matching();
}

MatchingAlgorithm::MatchedPairListType& PopularMatching::get_matched_pairs() {
    for (auto m : G_2_->get_A_partition()) {
        //auto& m_id = m->get_id();
        auto& partner_list = m->get_partners();

        for (auto i = partner_list.begin(), e = partner_list.end(); i != e; ++i) {
            //const auto& orig_id = get_original_id(u->get_id());
            //matched_pairs_.emplace_back(get_original_id(m_id),
            //        partner_list.get_vertex(i));
        }
    }

    return matched_pairs_;
}

