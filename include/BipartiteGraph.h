#ifndef BIPARTITE_GRAPH_H
#define BIPARTITE_GRAPH_H

#include <map>
#include <memory>
#include <ostream>
#include "TDefs.h"

class BipartiteGraph {
public:
    typedef std::map<IdType, VertexPtr> ContainerType;
    typedef ContainerType::iterator Iterator;

private:
    /// the partitions A and B
    ContainerType A_;
    ContainerType B_;

public:
    BipartiteGraph(const ContainerType& A, const ContainerType& B);
    virtual ~BipartiteGraph() = default;

    const ContainerType& get_A_partition() const;
    const ContainerType& get_B_partition() const;
    // bool has_augmenting_path(const MatchedPairListType& M) const;

    friend std::ostream& operator<<(std::ostream& out,
                                    std::shared_ptr<BipartiteGraph> G);
};

#endif
