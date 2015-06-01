#include "GraphReader.h"
#include "BipartiteGraph.h"
#include "PopularMatching2Sided.h"
#include <map>
#include <string>
#include <stdexcept>
#include <fstream>
#include <iostream>

void test_popular_matching_2_sided(const char* file_name) {
    // create the bipartite graph
    GraphReader reader(file_name);
    BipartiteGraph* G = reader.read_graph();
    // find the largest popular matching
    PopularMatching2Sided pm2s(G);
    PopularMatching2Sided::MatchedPairListType matched_pairs = pm2s.get_matched_pairs();
    std::cout << '{';
    for (PopularMatching2Sided::MatchedPairListType::iterator
            i = matched_pairs.begin(),
            e = matched_pairs.end();
         i != e; ++i)
    {
        PopularMatching2Sided::MatchedPairType const& mpair = *i;
        std::cout << "(" << mpair.first << ", " << mpair.second << ")";
        if (i + 1 != e) {
            std::cout << ",";
        } else {
            std::cout << "}\n";
        }
    }
    
    // free data structures
    delete G;
}

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " <graph file>\n";
        return 0;
    }
    
    test_popular_matching_2_sided(argv[1]);
    return 0;
}
