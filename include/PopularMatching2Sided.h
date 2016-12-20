#ifndef POPULAR_MATCHING_2_SIDED_H
#define POPULAR_MATCHING_2_SIDED_H

#include "Vertex.h"
#include "BipartiteGraph.h"
#include <map>
#include <vector>

/// Based on the following paper:
/// Popularity vs maximum cardinality in the stable marriage setting.
/// Kavitha, Telikepalli
/// Soda, 2012

class PopularMatching2Sided {
public:
    typedef std::pair<Vertex::IdType, Vertex::IdType> MatchedPairType;
    typedef std::vector<MatchedPairType> MatchedPairListType;
private:
    BipartiteGraph* G_; // the original bipartite graph
    std::map<Vertex::IdType, BipartiteGraph::VertexType> map_a1_; // vertices of set A with level 1
    MatchedPairListType matched_pair_list_; // to store the matched pairs
    
    /// modifies the graph according to the algorithm
    void modify_graph();
    void find_matching();

public:
    PopularMatching2Sided(BipartiteGraph* G);
    virtual ~PopularMatching2Sided();
    MatchedPairListType const& get_matched_pairs() const;
};

#endif
