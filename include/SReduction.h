#ifndef S_REDUCTION_H
#define S_REDUCTION_H

#include "BipartiteGraph.h"

std::unique_ptr<BipartiteGraph> augment_graph(
    const std::unique_ptr<BipartiteGraph>& G, int s);

#endif
