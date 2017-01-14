#include "PopularAmongMaxCard.h"
#include "SReduction.h"
#include "StableMarriage.h"

PopularAmongMaxCard::PopularAmongMaxCard(const std::unique_ptr<BipartiteGraph>& G,
                                         bool A_proposing)
    : MatchingAlgorithm(G), A_proposing_(A_proposing)
{}

PopularAmongMaxCard::~PopularAmongMaxCard() {
}

bool PopularAmongMaxCard::compute_matching() {
    const std::unique_ptr<BipartiteGraph>& G = get_graph();
    for (int s = 4; s <= 8; s += 2) {
        G_ = augment_graph(G, s);
        StableMarriage sm(G_, A_proposing_);
        sm.compute_matching();

        if (not G_->has_augmenting_path()) {
            return true;
        }
    }

    return false;
}

MatchingAlgorithm::MatchedPairListType& PopularAmongMaxCard::get_matched_pairs() {
    const std::unique_ptr<BipartiteGraph>& G = get_graph();

    for (auto it : G_->get_A_partition()) {
        auto& a = it.second;
        auto& partner_list = a->get_partners();

        for (auto i = partner_list.begin(), e = partner_list.end(); i != e; ++i) {
            auto b = partner_list.get_vertex(i);

            // if one of the matched partner is a dummy, do not add to the matching
            if (not a->is_dummy() and not b->is_dummy()) {
                // get the vertex from the original graph G
                auto orig_a = G->get_A_partition().at(a->get_cloned_for_id());
                auto orig_b = G->get_B_partition().at(b->get_cloned_for_id());

                // add them to the matching
                matched_pairs_.emplace_back(orig_a, orig_b);
            }
        }
    }

    return matched_pairs_;
}

