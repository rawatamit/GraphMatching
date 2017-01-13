#ifndef POPULAR_AMONG_MAX_CARD_H
#define POPULAR_AMONG_MAX_CARD_H

#include "MatchingAlgorithm.h"

class PopularAmongMaxCard : public MatchingAlgorithm {
private:
    std::unique_ptr<BipartiteGraph> G_;
    bool A_proposing_; // true if vertices from partition A propose, otherwise false

public:
    PopularAmongMaxCard(const std::unique_ptr<BipartiteGraph>& G,
                        bool A_proposing=true);
    virtual ~PopularAmongMaxCard();

    void compute_matching();
    MatchingAlgorithm::MatchedPairListType& get_matched_pairs();
};

#endif
