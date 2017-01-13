#ifndef POPULAR_MATCHING_H
#define POPULAR_MATCHING_H

#include "MatchingAlgorithm.h"

class PopularMatching : public MatchingAlgorithm {
private:
    std::unique_ptr<BipartiteGraph> G_;
    int s_; // how many priority increase does each vertex get
    bool A_proposing_; // true if vertices from partition A propose, otherwise false

public:
    PopularMatching(const std::unique_ptr<BipartiteGraph>& G,
                    int s, bool A_proposing=true);
    virtual ~PopularMatching();

    void compute_matching();
    MatchingAlgorithm::MatchedPairListType& get_matched_pairs();
};

#endif
