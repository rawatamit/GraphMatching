#include "GraphReader.h"
#include "BipartiteGraph.h"
#include "PartnerList.h"
#include "MatchingAlgorithm.h"
#include "StableMarriage.h"
#include "Popular.h"
#include "HeuristicHRLQ.h"
#include "Utils.h"
#include <stdexcept>
#include <iostream>
#include <unistd.h>

template<typename T>
void compute_matching(const char* input_file, const char* output_file) {
    GraphReader reader(input_file);
    std::unique_ptr<BipartiteGraph> G = reader.read_graph();

    T alg(G);
    if (alg.compute_matching()) {
        auto& M = alg.get_matched_pairs();
        std::ofstream out(output_file);
        print_matching(G, M, out);
    } else {
        throw std::runtime_error("unable to compute matching.");
    }
}

int main(int argc, char* argv[]) {
    int c = 0;
    bool compute_stable = false;
    bool compute_popular = false;
    bool compute_max_card = false;
    bool compute_hrlq = false;
    const char* input_file = nullptr;
    const char* output_file = nullptr;

    opterr = 0;
    while ((c = getopt(argc, argv, "spmhi:o:")) != -1) {
        switch (c) {
            case 's': compute_stable = true; break;
            case 'p': compute_popular = true; break;
            case 'm': compute_max_card = true; break;
            case 'h': compute_hrlq = true; break;
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
    } else if (compute_stable) {
        compute_matching<StableMarriage>(input_file, output_file);
    } else if (compute_popular) {
        compute_matching<MaxCardPopular>(input_file, output_file);
    } else if (compute_max_card) {
        compute_matching<PopularAmongMaxCard>(input_file, output_file);
    } else if (compute_hrlq) {
        compute_matching<HeuristicHRLQ>(input_file, output_file);
    }

    return 0;
}
