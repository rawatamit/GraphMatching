#include "Utils.h"
#include "Vertex.h"
#include <sstream>

// a new id is of the form id^k
IdType get_vertex_id(const IdType& id, int k) {
    std::stringstream stmp;
    stmp << id << '^' << k;
    return stmp.str();
}

// a dummy id is of the form d^k_id
IdType get_dummy_id(const IdType& id, int k) {
    std::stringstream stmp;
    stmp << "d^" << k << '_' << id;
    return stmp.str();
}

// return the dummy level from the given id
int get_dummy_level(const IdType& id) {
    auto caret_pos = id.find('^');
    auto underscore_pos = id.find('_');
    return std::atoi(id.substr(caret_pos+1, underscore_pos-caret_pos-1).c_str());
}

void print_matching_standard_format(MatchingAlgorithm::MatchedPairListType& matched_pairs,
                                    std::ostream& out)
{
    std::stringstream stmp;

    for (MatchingAlgorithm::MatchedPairListType::iterator
            i = matched_pairs.begin(),
            e = matched_pairs.end();
         i != e; ++i)
    {
        auto mpair = *i;
        auto a = mpair.first;
        auto b = mpair.second;
        auto& pref_list_a = a->get_preference_list();

        stmp << a->get_id() << ','
             << b->get_id() << ','
             << pref_list_a.get_rank(pref_list_a.find(b)) << '\n';
    }

    out << stmp.str();
}
