#ifndef POPULAR_H
#define POPULAR_H

#include "MatchingAlgorithm.h"

class SPopular : public MatchingAlgorithm {
private:
    std::unique_ptr<BipartiteGraph> G_;
    MatchedPairListType M_tmp_;
    // how many s values to try
    int sbegin_;
    int send_;
    bool A_proposing_; // true if vertices from partition A propose, otherwise false

public:
    SPopular(const std::unique_ptr<BipartiteGraph>& G,
             int sbegin, int send, bool A_proposing=true);
    virtual ~SPopular();

    bool compute_matching();
    MatchedPairListType& get_matched_pairs();
};

/// max-card popular matching in an HR instance
class MaxCardPopular : public SPopular {
private:
    std::unique_ptr<BipartiteGraph> G_;

public:
    MaxCardPopular(const std::unique_ptr<BipartiteGraph>& G,
                   bool A_proposing=true)
        : SPopular(G, 2, 2, A_proposing)
    {}

    virtual ~MaxCardPopular()
    {}
};

/// a popular matching among the set of max-card matchings in an instance
class PopularAmongMaxCard : public SPopular {
private:
    std::unique_ptr<BipartiteGraph> G_;

public:
    PopularAmongMaxCard(const std::unique_ptr<BipartiteGraph>& G,
                        bool A_proposing=true)
        : SPopular(G, 10, 10, A_proposing)
    {}

    virtual ~PopularAmongMaxCard()
    {}
};

#endif
