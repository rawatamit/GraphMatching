#ifndef BIPARTITE_GRAPH_H
#define BIPARTITE_GRAPH_H

#include "Vertex.h"
#include <set>
#include <ostream>

class BipartiteGraph {
public:
    typedef std::set<Vertex*> VertexSetType;
    typedef VertexSetType::iterator VertexSetIterator;

private:
    /// the partitions A and B
    VertexSetType A_;
    VertexSetType B_;

public:
    BipartiteGraph(VertexSetType const& A, VertexSetType const& B);
    virtual ~BipartiteGraph();
    
    VertexSetType const& get_set_A();
    VertexSetType const& get_set_B();
    
    Vertex* get_vertex_from_A(Vertex* v);
    Vertex* get_vertex_from_B(Vertex* v);
    
    friend std::ostream& operator<<(std::ostream& out, BipartiteGraph* G);
};

#endif
