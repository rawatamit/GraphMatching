#ifndef MATCHING_ALGORITHM_H
#define MATCHING_ALGORITHM_H

#include <ostream>
#include <vector>
#include "BipartiteGraph.h"

class MatchingAlgorithm {
public:
    typedef std::pair<std::shared_ptr<Vertex>, std::shared_ptr<Vertex>>
        MatchedPair;
    typedef std::vector<MatchedPair> MatchedPairListType;

protected:
    MatchedPairListType matched_pairs_;

private:
    const std::unique_ptr<BipartiteGraph>& G_;

public:
    MatchingAlgorithm(const std::unique_ptr<BipartiteGraph>& G);
    virtual ~MatchingAlgorithm();
    virtual void compute_matching() = 0;
    virtual MatchedPairListType& get_matched_pairs();
    const std::unique_ptr<BipartiteGraph>& get_graph() const;
};

std::ostream& operator<<(std::ostream& out,
                         MatchingAlgorithm::MatchedPairListType& matched_pairs);

#endif
