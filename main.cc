#include "BipartiteGraph.h"
#include "StableMarriage.h"
#include "Popular.h"
#include "RHeuristicHRLQ.h"
#include "HHeuristicHRLQ.h"
#include "YokoiEnvyfreeHRLQ.h"
#include "MaximalEnvyfreeHRLQ.h"
#include "Utils.h"
#include "GraphReader.h"
#include <fstream>
#include <iostream>
#include <unistd.h>

template<typename T>
void compute_matching(bool A_proposing, const char* input_file, const char* output_file) {
    // setup input/output stream as std::cin/std::cout by default
    // if a file is specified use it to read/write

    auto cin_buf = std::cin.rdbuf(); // save pointer to std::cin buffer
    auto cout_buf = std::cout.rdbuf(); // save pointer to std::cout buffer

    std::ifstream filein(input_file);
    std::ofstream fileout(output_file);

    if (input_file) {
        std::cin.rdbuf(filein.rdbuf());
    }

    if (output_file) {
        std::cout.rdbuf(fileout.rdbuf());
    }

    std::shared_ptr<BipartiteGraph> G = GraphReader(std::cin).read_graph();
    T alg(G, A_proposing);
    auto M = alg.compute_matching();
    print_matching(G, M, std::cout);

    // restore buffers
    std::cin.rdbuf(cin_buf);
    std::cout.rdbuf(cout_buf);
}

int main(int argc, char* argv[]) {
    int c = 0;
    bool compute_stable = false;
    bool compute_popular = false;
    bool compute_max_card = false;
    bool compute_rhrlq = false;
    bool compute_hhrlq = false;
    bool compute_yhrlq = false;
    bool compute_ehrlq = false;
    bool A_proposing = true;
    const char* input_file = nullptr;
    const char* output_file = nullptr;

    opterr = 0;
    // choose the proposing partition using -A and -B
    // -s, -p, and -m flags compute the stable, max-card popular and pop among
    // max-card matchings respectively
    // -r and -h compute the resident and hopsital heuristic for an HRLQ instance
    // -i is the path to the input graph, -o is the path where the matching
    // computed should be stored
    while ((c = getopt(argc, argv, "ABspmrhyei:o:")) != -1) {
        switch (c) {
            case 'A': A_proposing = true; break;
            case 'B': A_proposing = false; break;
            case 's': compute_stable = true; break;
            case 'p': compute_popular = true; break;
            case 'm': compute_max_card = true; break;
            case 'r': compute_rhrlq = true; break;
            case 'h': compute_hhrlq = true; break;
            case 'y': compute_yhrlq = true; break;
            case 'e': compute_ehrlq = true; break;
            case 'i': input_file = optarg; break;
            case 'o': output_file = optarg; break;
            case '?':
                if (optopt == 'i') {
                    std::cerr << "Option -i requires an argument.\n";
                } else if (optopt == 'o') {
                    std::cerr << "Option -o requires an argument.\n";
                } else {
                    std::cerr << "Unknown option: " << static_cast<char>(optopt) << '\n';
                }
                break;
            default: break;
        }
    }

    if (compute_stable) {
        compute_matching<StableMarriage>(A_proposing, input_file, output_file);
    } else if (compute_popular) {
        compute_matching<MaxCardPopular>(A_proposing, input_file, output_file);
    } else if (compute_max_card) {
        compute_matching<PopularAmongMaxCard>(A_proposing, input_file, output_file);
    } else if (compute_rhrlq) {
        compute_matching<RHeuristicHRLQ>(A_proposing, input_file, output_file);
    } else if (compute_hhrlq) {
        compute_matching<HHeuristicHRLQ>(A_proposing, input_file, output_file);
    } else if (compute_yhrlq) {
        compute_matching<YokoiEnvyfreeHRLQ>(A_proposing, input_file, output_file);
    } else if (compute_ehrlq) {
        compute_matching<MaximalEnvyfreeHRLQ>(A_proposing, input_file, output_file);
    }

    return 0;
}
