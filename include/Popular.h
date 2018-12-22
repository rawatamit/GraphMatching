#ifndef POPULAR_H
#define POPULAR_H

#include "MatchingAlgorithm.h"
#include "NProposingMatching.h"

/// max-card popular matching in an HR instance
class MaxCardPopular : public MatchingAlgorithm {
public:
    explicit MaxCardPopular(const std::unique_ptr<BipartiteGraph>& G,
                   bool A_proposing=true)
        : MatchingAlgorithm(G, A_proposing)
    {}

    ~MaxCardPopular() override = default;

    bool compute_matching() override {
        NProposingMatching npm (get_graph(), is_A_proposing(), 1);
        bool computed = npm.compute_matching();
        M_ = map_inverse(npm.get_matched_pairs());
        return computed;
    }
};

/// a popular matching among the set of max-card matchings in an instance
class PopularAmongMaxCard  : public MatchingAlgorithm {
public:
    explicit PopularAmongMaxCard(const std::unique_ptr<BipartiteGraph>& G,
                   bool A_proposing=true)
        : MatchingAlgorithm(G, A_proposing)
    {}

    ~PopularAmongMaxCard() override = default;

    bool compute_matching() override {
        NProposingMatching npm (get_graph(), is_A_proposing(), (int) get_graph()->get_A_partition().size() - 1);
        bool computed = npm.compute_matching();
        M_ = npm.get_matched_pairs();
        return computed;
    }
};

#endif
