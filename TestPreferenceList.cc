#include "catch.hpp"
#include "Vertex.h"
#include "PreferenceList.h"

TEST_CASE("preference list", "[pref_list]") {
    PreferenceList pref_list;
    std::shared_ptr<Vertex> v1 = "v1";
    std::shared_ptr<Vertex> v2 = "v2";
    std::shared_ptr<Vertex> v2 = "v2";

    REQUIRE(pref_list.empty());

    SECTION("adding vertices to the pref_list_") {
        pref_list.emplace_back(v1);
        REQUIRE_FALSE(pref_list.empty());
    }
}

