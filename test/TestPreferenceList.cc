#include "catch.hpp"
#include "Vertex.h"
#include "PreferenceList.h"
#include <memory>

TEST_CASE("preference list 1", "[pref_list]") {
    PreferenceList pref_list;
    std::shared_ptr<Vertex> v1 = std::make_shared<Vertex>("v1");

    REQUIRE(pref_list.empty());

    SECTION("adding vertices to the pref_list_") {
        pref_list.emplace_back(v1);
        REQUIRE_FALSE(pref_list.empty());
    }
}

TEST_CASE("preference list 2", "[pref_list]") {
    PreferenceList pref_list;
    std::shared_ptr<Vertex> v1 = std::make_shared<Vertex>("v1");
    std::shared_ptr<Vertex> v2 = std::make_shared<Vertex>("v2");
    std::shared_ptr<Vertex> v3 = std::make_shared<Vertex>("v3");

    REQUIRE(pref_list.empty());

    pref_list.emplace_back(v1);
    pref_list.emplace_back(v2);
    pref_list.emplace_back(v3);

    REQUIRE(pref_list.size() == 3);
}