#include "GraphReader.h"
#include "BipartiteGraph.h"
#include "MatchingAlgorithm.h"
#include "StableMarriage.h"
#include "PopularMatching.h"
#include "PopularAmongMaxCard.h"
#include "SReduction.h"
#include "Utils.h"
#include <map>
#include <string>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <unistd.h>

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

void generate_stable_matching(const char* input_file, const char* output_file) {
    // create the bipartite graph
    GraphReader reader(input_file);
    std::unique_ptr<BipartiteGraph> G = reader.read_graph();

    StableMarriage sm(G);
    sm.compute_matching();
    MatchingAlgorithm::MatchedPairListType matched_pairs = sm.get_matched_pairs();
    std::ofstream out(output_file);
    print_matching_standard_format(matched_pairs, out);
}

void generate_popular_matching(const char* input_file, const char* output_file) {
    // create the bipartite graph
    GraphReader reader(input_file);
    std::unique_ptr<BipartiteGraph> G = reader.read_graph();

    // find the maximum cardinality popular matching
    PopularMatching pm(G, 2);
    //PopularMatching pm(G, G->get_A_partition().size());
    pm.compute_matching();
    MatchingAlgorithm::MatchedPairListType matched_pairs = pm.get_matched_pairs();
    std::ofstream out(output_file);
    print_matching_standard_format(matched_pairs, out);
}

void generate_popular_among_max_card(const char* input_file, const char* output_file) {
    // create the bipartite graph
    GraphReader reader(input_file);
    std::unique_ptr<BipartiteGraph> G = reader.read_graph();

    // find the maximum cardinality popular matching
    PopularAmongMaxCard pm(G, 1);
    //PopularMatching pm(G, G->get_A_partition().size());
    pm.compute_matching();
    MatchingAlgorithm::MatchedPairListType matched_pairs = pm.get_matched_pairs();
    std::ofstream out(output_file);
    print_matching_standard_format(matched_pairs, out);
}

int main(int argc, char* argv[]) {
    // parse options
    int c = 0;
    bool generate_stable = false;
    bool generate_popular = false;
    bool generate_max_card = false;
    const char* input_file = nullptr;
    const char* output_file = nullptr;

    opterr = 0;
    while ((c = getopt(argc, argv, "spmi:o:")) != -1) {
        switch (c) {
            case 's': generate_stable = true; break;
            case 'p': generate_popular = true; break;
            case 'm': generate_max_card = true; break;
            case 'i': input_file = optarg; break;
            case 'o': output_file = optarg; break;
            case '?':
                if (optopt == 'i') {
                    std::cerr << "Option -i requires an argument.\n";
                } else if (optopt == 'o') {
                    std::cerr << "Option -o requires an argument.\n";
                } else {
                    std::cerr << "Unknown option: " << optopt << '\n';
                }
                break;
            default: break;
        }
    }

    // do not proceed if file names are not valid
    if (not input_file or not output_file) {
    } else if (generate_stable) {
        generate_stable_matching(input_file, output_file);
    } else if (generate_popular) {
        generate_popular_matching(input_file, output_file);
    } else if (generate_max_card) {
        generate_popular_among_max_card(input_file, output_file);
    }

    return 0;
}
