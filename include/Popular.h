#ifndef POPULAR_H
#define POPULAR_H

#include "MatchingAlgorithm.h"
#include "NProposingMatching.h"

/// max-card popular matching in an HR instance
class MaxCardPopular : public MatchingAlgorithm {
public:
    explicit MaxCardPopular(std::shared_ptr<BipartiteGraph> G,
                   bool A_proposing=true)
        : MatchingAlgorithm(G, A_proposing)
    {}

    ~MaxCardPopular() override = default;

    Matching compute_matching() override {
        NProposingMatching npm (get_graph(), is_A_proposing(), 1);
        return npm.compute_matching();
    }
};

/// a popular matching among the set of max-card matchings in an instance
class PopularAmongMaxCard  : public MatchingAlgorithm {
public:
    explicit PopularAmongMaxCard(std::shared_ptr<BipartiteGraph> G,
                   bool A_proposing=true)
        : MatchingAlgorithm(G, A_proposing)
    {}

    ~PopularAmongMaxCard() override = default;

    Matching compute_matching() override {
        NProposingMatching npm (get_graph(), is_A_proposing(), 10);//(int) get_graph()->get_A_partition().size() - 1);
        return npm.compute_matching();
    }
};

/// max-card popular matching in an many-many LQ instance
class MaxCardPopularLQ : public MatchingAlgorithm {
private:
    void decide_accept_reject(
        VertexPtr u, int cap_u,
        VertexPtr v, int cap_v,
        VertexBookkeeping& u_data,
        const PreferenceList& v_pref_list, FreeListType& free_list,
        std::map<VertexPtr, VertexBookkeeping>& bookkeep_data,
        Matching& M);

public:
    explicit MaxCardPopularLQ(std::shared_ptr<BipartiteGraph> G,
                              bool A_proposing=true)
        : MatchingAlgorithm(G, A_proposing)
    {}

    ~MaxCardPopularLQ() override = default;

    // Compute HR2LQ max-cardinality popular matching from set of critical
    // matchings.
    Matching compute_matching() override;
};

#endif
