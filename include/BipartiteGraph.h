#ifndef BIPARTITE_GRAPH_H
#define BIPARTITE_GRAPH_H

#include <memory>
#include <ostream>
#include <set>

class Vertex;  // forward definition for Vertex

class BipartiteGraph {
public:
    typedef std::shared_ptr<Vertex> VertexType;
    typedef std::set<VertexType> VertexSetType;
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

    VertexType get_vertex_from_A(const VertexType& v);
    VertexType get_vertex_from_B(const VertexType& v);

    friend std::ostream& operator<<(std::ostream& out,
                                    const std::unique_ptr<BipartiteGraph>& G);
};

#endif
