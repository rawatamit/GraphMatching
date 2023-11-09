#ifndef CRITICAL_RSM_H
#define CRITICAL_RSM_H

#include "MatchingAlgorithm.h"
#include "NProposingTiesMatching.h"

// Critical relaxed stable matching is critical as well as relaxed stable
// A matching M is critical if there is no other matching that matches 
// more critical vertices than M
// A matching M is relaxed stable if for every blocking pair (a, b) 
// w.r.t. M one of the following holds:
// 1. a is matched and b' = M(a) is critical, or
// 2. b is matched and a′ = M(b) is critical
class CriticalRSM : public MatchingAlgorithm {
private:
    VertexPtr favourite_neighbour(VertexPtr u, const PreferenceList& u_pref_list, VertexBookkeeping &u_data, const Matching& M);
    void ties_propose(FreeListType& free_list, VertexPtr a, PreferenceList& a_pref_list, std::map<VertexPtr, VertexBookkeeping>& bookkeep_data, Matching M, int t);
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
