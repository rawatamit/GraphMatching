#include "Vertex.h"
#include "BipartiteGraph.h"
#include "PopularMatching2Sided.h"
#include <map>
#include <string>
#include <stdexcept>
#include <fstream>
#include <iostream>

typedef std::map<Vertex::IdType, Vertex*> VertexMap;

//// TODO: these are very buggy file reading primitives,
//// implement better ones, maybe write one in bison

/// splits a string such as "a, b, c" into a, b and c
void split_string_on_comma(std::string const& s, std::vector<std::string>& v) {
    std::string::size_type pindex = 0, cindex = 0;
    
    // read the comma separated list
    while (cindex != std::string::npos) {
        // skip whitespace
        while (isspace(s[pindex])) ++pindex;
        
        cindex = s.find(",", pindex);
        if (cindex != std::string::npos) {
            v.emplace_back(s.substr(pindex, cindex - pindex));
            pindex = cindex + 1;
        }
    }
    
    v.emplace_back(s.substr(pindex, s.size() - pindex));
}

void read_vertex_set(VertexMap& vmap, std::ifstream& in) {
    std::string line;
    std::getline(in, line);
    std::vector<std::string> v;
    split_string_on_comma(line, v);
    
    for (auto s : v) {
        vmap[s] = new Vertex(s);
    }
}

void read_preference_lists(VertexMap& vmapA, VertexMap& vmapB, std::ifstream& in) {
    std::string line;
    
    while (std::getline(in, line) and line != "@End") {
        std::vector<std::string> v;
        
        std::string::size_type colon_index = line.find(":");
        std::string vertex = line.substr(0, colon_index);
        split_string_on_comma(line.substr(colon_index+1, line.size()), v);
        
        Vertex* vx = nullptr;
        bool mapA = false;
        if (vmapA.find(vertex) != vmapA.end()) {
            vx = vmapA[vertex];
            mapA = true;
        } else {
            vx = vmapB[vertex];
        }
        
        Vertex::PreferenceListType pref_list;
        for (auto s : v) {
            if (mapA) {
                pref_list.emplace_front(vmapB[s]);
            } else {
                pref_list.emplace_front(vmapA[s]);
            }
        }
        vx->set_preference_list(pref_list);
    }
}

/// file format is specified in example_paper.txt
BipartiteGraph* read_graph(const char* file_name) {
    std::ifstream in(file_name);
    if (in) {
        std::string line;
        VertexMap vmapA, vmapB;
        
        while (std::getline(in, line) and line != "@End") {
            // ignore blank lines and comments
            if (line.size() != 0 and line[0] != '#') {
                if (line == "@BipartitionA") {
                    read_vertex_set(vmapA, in);
                } else if (line == "@BipartitionB") {
                    read_vertex_set(vmapB, in);
                } else if (line == "@PreferenceLists") {
                    read_preference_lists(vmapA, vmapB, in);
                }
            }
        }
        
        BipartiteGraph::VertexSetType A;
        BipartiteGraph::VertexSetType B;
        for (auto& it : vmapA) {
            A.insert(it.second);
        }
        
        for (auto& it : vmapB) {
            B.insert(it.second);
        }
        
        return new BipartiteGraph(A, B);
    } else {
        throw std::runtime_error("error reading file");
    }
}

void test_popular_matching_2_sided(const char* file_name) {
    // create the bipartite graph
    BipartiteGraph* G = read_graph(file_name);
    
#if 0
    std::cout << std::boolalpha << a1->prefers(b1, b2) << '\n';
    std::cout << std::boolalpha << a1->prefers(b2, b1) << '\n';
    std::cout << std::boolalpha << b1->prefers(a1, a1) << '\n';
    std::cout << std::boolalpha << b1->prefers(a1, a2) << '\n';
#endif

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
