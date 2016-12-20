#ifndef MATCHING_ALGORITHM_H
#define MATCHING_ALGORITHM_H

#include <vector>
#include "BipartiteGraph.h"

class MatchingAlgorithm {
public:
    typedef std::pair<std::shared_ptr<Vertex>, std::shared_ptr<Vertex>>
        MatchedPair;
    typedef std::vector<MatchedPair> MatchedPairListType;

private:
    MatchedPairListType matched_pairs_;
    const std::unique_ptr<BipartiteGraph>& G_;

public:
    MatchingAlgorithm(const std::unique_ptr<BipartiteGraph>& G);
    virtual ~MatchingAlgorithm();
    virtual void compute_matching();
    const std::unique_ptr<BipartiteGraph>& get_graph() const;
    MatchedPairListType const& get_matched_pairs() const;
};

#endif
