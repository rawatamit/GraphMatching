#ifndef MAX_CARD_POPULAR_H
#define MAX_CARD_POPULAR_H

#include "MatchingAlgorithm.h"

class MaxCardPopular : public MatchingAlgorithm {
private:
    std::unique_ptr<BipartiteGraph> G_;
    bool A_proposing_; // true if vertices from partition A propose, otherwise false

public:
    MaxCardPopular(const std::unique_ptr<BipartiteGraph>& G,
                    bool A_proposing=true);
    virtual ~MaxCardPopular();

    bool compute_matching();
    MatchingAlgorithm::MatchedPairListType& get_matched_pairs();
};

#endif
