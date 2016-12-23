#ifndef POPULAR_MATCHING_H
#define POPULAR_MATCHING_H

#include "MatchingAlgorithm.h"

class PopularMatching : public MatchingAlgorithm {
private:
    std::unique_ptr<BipartiteGraph> G_2_;

public:
    PopularMatching(const std::unique_ptr<BipartiteGraph>& G);
    virtual ~PopularMatching();

    void compute_matching();
    MatchingAlgorithm::MatchedPairListType& get_matched_pairs();
};

#endif
