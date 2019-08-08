#include "Partner.h"
#include "Vertex.h"

bool operator<(const Partner& a, const Partner& b) {
    // if level of a is less than of b
    // a is less preferred
    if (a.level < b.level) {
        return true;
    } else if (a.level > b.level) {
        return false;
    } else {
        // however if a and b have same level
        // and a's rank is > than b's rank
        // a is less preferred
        return a.rank > b.rank;
    }
}

std::ostream& operator<<(std::ostream& out, const Partner& a) {
    return out << '(' << "id: " << a.vertex->get_id() << ", rank: " << a.rank << ", level: " << a.level << ')';
}
