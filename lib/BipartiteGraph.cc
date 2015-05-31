#include "BipartiteGraph.h"
#include <algorithm>

BipartiteGraph::BipartiteGraph(VertexSetType const& A, VertexSetType const& B)
    : A_(A), B_(B)
{
}

BipartiteGraph::~BipartiteGraph() {
    A_.clear();
    B_.clear();
}

BipartiteGraph::VertexSetType const& BipartiteGraph::get_set_A() {
    return A_;
}

BipartiteGraph::VertexSetType const& BipartiteGraph::get_set_B() {
    return B_;
}

Vertex* BipartiteGraph::get_vertex_from_A(Vertex* v) {
    VertexSetIterator it = A_.find(v);
    return it == A_.end() ? nullptr : *it;
}

Vertex* BipartiteGraph::get_vertex_from_B(Vertex* v) {
    VertexSetIterator it = B_.find(v);
    return it == B_.end() ? nullptr : *it;
}

std::ostream& operator<<(std::ostream& out, BipartiteGraph* G) {
    out << "A:\n";
    for (Vertex* v : G->get_set_A()) {
        out << "    " << v;
    }
    
    out << "B:\n";
    for (Vertex* v : G->get_set_B()) {
        out << "    " << v;
    }
    
    return out;
}
