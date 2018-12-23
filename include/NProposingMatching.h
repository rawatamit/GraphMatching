#ifndef N_PROPOSING_MATCHING_H
#define N_PROPOSING_MATCHING_H

#include "MatchingAlgorithm.h"
#include "PreferenceList.h"

// simulate Gale and Shapley with n levels for proposing side
class NProposingMatching : public MatchingAlgorithm {
private:
    // keep track of preference list indices for a vertex
    struct PrefListBounds {
        // [begin, end)
        // begin is also the proposal index
        PreferenceList::SizeType begin;
        PreferenceList::SizeType end;

        PrefListBounds()
            : begin(0), end(0)
        {}

        PrefListBounds(PreferenceList::SizeType begin, PreferenceList::SizeType end)
            : begin(begin), end(end)
        {}

        bool is_exhausted() {
            return begin >= end;
        }
    };

    int max_level;     // maximum level that a vertex can reach

    // add (u, v) pair to M_
    void add_matched_partners(std::shared_ptr<MatchedPairListType> M,
                              VertexPtr u, VertexPtr v,
                              int u_level,
                              const PrefListBounds& u_pref_list_bounds,
                              const PreferenceList& v_pref_list);

public:
    explicit NProposingMatching(std::shared_ptr<BipartiteGraph> G, bool A_proposing, int max_level);
    ~NProposingMatching() override = default;

    std::shared_ptr<MatchedPairListType> compute_matching() override;
};

#endif
