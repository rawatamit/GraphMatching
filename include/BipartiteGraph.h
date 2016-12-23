#ifndef BIPARTITE_GRAPH_H
#define BIPARTITE_GRAPH_H

#include <memory>
#include <ostream>
#include <vector>
#include "Vertex.h"

class Vertex;  // forward definition for Vertex

class BipartiteGraph {
public:
    typedef std::vector<VertexPtr> VertexSetType;
    typedef VertexSetType::iterator VertexSetIterator;

private:
    /// the partitions A and B
    VertexSetType A_;
    VertexSetType B_;

public:
    BipartiteGraph(const VertexSetType& A, const VertexSetType& B);
    virtual ~BipartiteGraph();

    const VertexSetType& get_A_partition();
    const VertexSetType& get_B_partition();

    friend std::ostream& operator<<(std::ostream& out,
                                    const std::unique_ptr<BipartiteGraph>& G);
};

#endif
