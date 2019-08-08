#ifndef PARTNER_H
#define PARTNER_H

#include "TDefs.h"

struct Partner {
    VertexPtr vertex;
    RankType rank;
    int level;

    Partner(VertexPtr vertex, RankType rank, int level)
        : vertex(vertex), rank(rank), level(level)
    {}

    // is this smaller that the other Partner
    // is this less preferred
    friend bool operator<(const Partner& a, const Partner& b);
    friend std::ostream& operator<<(std::ostream& out, const Partner& a);
};

#endif //PARTNER_H
