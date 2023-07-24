#include "BipartiteGraph.h"
#include "StableMarriage.h"
#include "Popular.h"
#include "YokoiEnvyfreeHRLQ.h"
#include "MaximalEnvyfreeHRLQ.h"
#include "Utils.h"
#include "GraphReader.h"
#include "Matching.h"
#include <fstream>
#include <iostream>
#include <unistd.h>

template<typename T>
bool compute_matching(bool A_proposing, const char* input_file,
                      const char* output_file, bool verify=false) {
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
    bool ret = true;

    if (verify)
    {
        ret = M.verify(G);
    }

    // Only print matching if it is verified to be correct.
    if (ret)
    {
        print_matching(G, M, std::cout);
    }

    // restore buffers
    std::cin.rdbuf(cin_buf);
    std::cout.rdbuf(cout_buf);

    return ret;
}

int main(int argc, char* argv[]) {
    int c = 0;
    bool compute_stable = false;
    bool compute_popular = false;
    bool compute_max_card = false;
    bool compute_yhrlq = false;
    bool compute_ehrlq = false;
    bool compute_popular_lq = false;
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
    while ((c = getopt(argc, argv, "ABspmyeli:o:")) != -1) {
        switch (c) {
            case 'A': A_proposing = true; break;
            case 'B': A_proposing = false; break;
            case 's': compute_stable = true; break;
            case 'p': compute_popular = true; break;
            case 'm': compute_max_card = true; break;
            case 'y': compute_yhrlq = true; break;
            case 'e': compute_ehrlq = true; break;
            case 'l': compute_popular_lq = true; break;
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

    bool status = true;
    if (compute_stable) {
        status = compute_matching<StableMarriage>(A_proposing, input_file, output_file);
    } else if (compute_popular) {
        status = compute_matching<MaxCardPopular>(A_proposing, input_file, output_file, true);
    } else if (compute_max_card) {
        status = compute_matching<PopularAmongMaxCard>(A_proposing, input_file, output_file);
    } else if (compute_yhrlq) {
        status = compute_matching<YokoiEnvyfreeHRLQ>(A_proposing, input_file, output_file);
    } else if (compute_ehrlq) {
        status = compute_matching<MaximalEnvyfreeHRLQ>(A_proposing, input_file, output_file);
    } else if (compute_popular_lq) {
        status = compute_matching<MaxCardPopularLQ>(A_proposing, input_file, output_file);
    }

    return status ? 0 : 1;
}
