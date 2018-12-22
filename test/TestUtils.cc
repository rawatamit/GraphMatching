#include "catch.hpp"
#include "Utils.h"
#include "Vertex.h"

TEST_CASE("Utils compute_rank", "[compute_rank]") {
    PreferenceList pref_list;
    auto v1 = std::make_shared<Vertex>("v1");
    auto v2 = std::make_shared<Vertex>("v2");

    pref_list.emplace_back(v1);
    pref_list.emplace_back(v2);

    REQUIRE(compute_rank(v1, pref_list, 0) == 1);
    REQUIRE(compute_rank(v2, pref_list, 0) == 2);
    REQUIRE(compute_rank(v1, pref_list, 1) == 3);
    REQUIRE(compute_rank(v2, pref_list, 1) == 4);
    REQUIRE(compute_rank(v1, pref_list, 2) == 5);
    REQUIRE(compute_rank(v2, pref_list, 2) == 6);
}
