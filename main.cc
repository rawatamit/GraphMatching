#include "GraphReader.h"
#include "BipartiteGraph.h"
#include "MatchingAlgorithm.h"
#include "StableMarriage.h"
#include <map>
#include <string>
#include <stdexcept>
#include <fstream>
#include <iostream>

#if 0
bool is_stable(MatchingAlgorithm::MatchedPairListType matched_pairs) {
    for (auto mpair : matched_pairs) {
        auto a = mpair.first;
        auto b = mpair.second;

        // for every woman ranked higher than a's matched partner (i.e. b)
        for (auto iter = a->get_preference_list().begin();
             iter->second != b and iter != a->get_preference_list().end();
             ++iter)
        {
            auto b_ = *iter;
            if (b_->is_matched() and // only if b_ is matched to someone
                a->is_higher_ranked_than(b_, b) and // a prefers b_ to b
                b_->is_higher_ranked_than(a, b_->get_partner())) // and b_ prefers a to M(b_)
            {
                return false;
            }
        }
    }

    return true;
}
#endif

void test_stable_matching(const char* file_name) {
    // create the bipartite graph
    GraphReader reader(file_name);
    std::unique_ptr<BipartiteGraph> G = reader.read_graph();
    //std::cout << G << '\n';

#if 1
    // find the largest popular matching
    StableMarriage sm(G);
    sm.compute_matching();
    MatchingAlgorithm::MatchedPairListType matched_pairs = sm.get_matched_pairs();

    std::cout << '{';
    for (MatchingAlgorithm::MatchedPairListType::iterator
            i = matched_pairs.begin(),
            e = matched_pairs.end();
         i != e; ++i)
    {
        auto mpair = *i;
        std::cout << "(" << mpair.first->get_id() << ", "
                  << mpair.second->get_id() << ")";
        if (i + 1 != e) {
            std::cout << ",";
        } else {
            std::cout << "}\n";
        }
    }
#endif
}

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " <graph file>\n";
        return 0;
    }

    test_stable_matching(argv[1]);
    return 0;
}
