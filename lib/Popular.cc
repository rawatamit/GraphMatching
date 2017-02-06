#include "Popular.h"
#include "Vertex.h"
#include "PartnerList.h"
#include "SReduction.h"
#include "StableMarriage.h"
#include "Utils.h"
#include <stack>
#include <algorithm>

SPopular::SPopular(const std::unique_ptr<BipartiteGraph>& G,
                   int sbegin, int send, bool A_proposing)
    : MatchingAlgorithm(G), sbegin_(sbegin), send_(send), A_proposing_(A_proposing)
{}

SPopular::~SPopular()
{}

#include <iostream>
bool SPopular::compute_matching() {
    const std::unique_ptr<BipartiteGraph>& G = get_graph();
    send_ = std::min(send_, (int)G->get_B_partition().size());
    sbegin_ = send_;
        // G_ = augment_graph(G, 20);//send_);
        // StableMarriage sm(G_, A_proMposing_);
        // auto& M = sm.get_matched_pairs();
        // auto& M_inv = map_inverse(M);
    // std::cerr << "is_aug_path " << G->has_augmenting_path(M_inv) << '\n';
        // return true;

    for (int s = sbegin_; s <= send_; s += 2) {
        G_ = augment_graph(G, s);
        StableMarriage sm(G_, A_proposing_);

        if (sm.compute_matching()) {
            const auto& M = sm.get_matched_pairs();

            // caller simply wants a matching for level-s
            if (sbegin_ == send_ or not G_->has_augmenting_path(M)) {
                M_tmp_ = M;
                return true;
            }
        } else {
            // unable to compute stable matching
            return false;
        }
    }

    return false;
}

MatchedPairListType& SPopular::get_matched_pairs() {
    return map_inverse(M_tmp_);
}

