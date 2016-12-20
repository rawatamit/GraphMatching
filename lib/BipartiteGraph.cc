#include "BipartiteGraph.h"
#include "Vertex.h"
#include <algorithm>

BipartiteGraph::BipartiteGraph(const VertexSetType& A, const VertexSetType& B)
    : A_(A), B_(B)
{}

BipartiteGraph::~BipartiteGraph() {
    A_.clear();
    B_.clear();
}

const BipartiteGraph::VertexSetType& BipartiteGraph::get_A_partition() {
    return A_;
}

const BipartiteGraph::VertexSetType& BipartiteGraph::get_B_partition() {
    return B_;
}

BipartiteGraph::VertexType BipartiteGraph::get_vertex_from_A(const VertexType& v) {
    VertexSetIterator it = A_.find(v);
    return it == A_.end() ? nullptr : *it;
}

BipartiteGraph::VertexType BipartiteGraph::get_vertex_from_B(const VertexType& v) {
    VertexSetIterator it = B_.find(v);
    return it == B_.end() ? nullptr : *it;
}

std::ostream& operator<<(std::ostream& out, const std::unique_ptr<BipartiteGraph>& G) {
    out << "A:\n";
    for (BipartiteGraph::VertexType const& v : G->get_A_partition()) {
        out << "    " << v;
    }

    out << "B:\n";
    for (BipartiteGraph::VertexType const& v : G->get_B_partition()) {
        out << "    " << v;
    }

    return out;
}
