#ifndef CRITICAL_RSM_H
#define CRITICAL_RSM_H

#include "MatchingAlgorithm.h"
#include "NProposingTiesMatching.h"

// critical relaxed stable matching
class CriticalRSM : public MatchingAlgorithm {
public:
    explicit CriticalRSM(std::shared_ptr<BipartiteGraph> G,
                            bool A_proposing=true)
    : MatchingAlgorithm(G, A_proposing)
    {}

    ~CriticalRSM() override = default;

    Matching compute_matching() override {
        NProposingTiesMatching nptm (get_graph(), is_A_proposing(), 0);
        return nptm.compute_matching();
    }
};

#endif
