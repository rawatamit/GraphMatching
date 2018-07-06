#ifndef POPULAR_H
#define POPULAR_H

#include "MatchingAlgorithm.h"

/// max-card popular matching in an HR instance
class MaxCardPopular : public MatchingAlgorithm {
private:
    bool A_proposing_; // true if vertices from partition A propose, otherwise false

public:
    MaxCardPopular(const std::unique_ptr<BipartiteGraph>& G,
                   bool A_proposing=true)
        : MatchingAlgorithm(G), A_proposing_(A_proposing)
    {}

    virtual ~MaxCardPopular()
    {}
    
    bool compute_matching() {
        NProposingMatching npm (get_graph(), A_proposing_, 2);
        bool computed = npm.compute_matching();
        M_ = npm.get_matched_pairs();
        return computed;
    }
};

/// a popular matching among the set of max-card matchings in an instance
class PopularAmongMaxCard  : public MatchingAlgorithm {
private:
    bool A_proposing_; // true if vertices from partition A propose, otherwise false

public:
    PopularAmongMaxCard(const std::unique_ptr<BipartiteGraph>& G,
                   bool A_proposing=true)
        : MatchingAlgorithm(G), A_proposing_(A_proposing)
    {}

    virtual ~PopularAmongMaxCard()
    {}
    
    bool compute_matching() {
        NProposingMatching npm (get_graph(), A_proposing_, get_graph()->get_A_partition().size());
        bool computed = npm.compute_matching();
        M_ = npm.get_matched_pairs();
        return computed;
    }
};

#endif
