#include "catch.hpp"
#include "Vertex.h"
#include "PreferenceList.h"
#include <memory>

TEST_CASE("PreferenceList empty", "[pref_list]") {
    PreferenceList pref_list;
    std::shared_ptr<Vertex> v1 = std::make_shared<Vertex>("v1");

    REQUIRE(pref_list.empty());

    SECTION("adding vertices to the pref_list") {
        pref_list.emplace_back(v1);
        REQUIRE_FALSE(pref_list.empty());
    }
}

TEST_CASE("PreferenceList find", "[pref_list]") {
    PreferenceList pref_list;
    std::shared_ptr<Vertex> v1 = std::make_shared<Vertex>("v1");
    std::shared_ptr<Vertex> v2 = std::make_shared<Vertex>("v2");
    std::shared_ptr<Vertex> v3 = std::make_shared<Vertex>("v3");
    std::shared_ptr<Vertex> v4 = std::make_shared<Vertex>("v4"); // not in pref list

    pref_list.emplace_back(v1);
    pref_list.emplace_back(v2);
    pref_list.emplace_back(v3);

    REQUIRE(pref_list.size() == 3);

    SECTION("find") {
        REQUIRE(pref_list.find(v1)->vertex == v1);
        REQUIRE(pref_list.find(v1)->rank == 1);

        REQUIRE(pref_list.find(v2)->vertex == v2);
        REQUIRE(pref_list.find(v2)->rank == 2);

        REQUIRE(pref_list.find(v3)->vertex == v3);
        REQUIRE(pref_list.find(v3)->rank == 3);

        REQUIRE(pref_list.find(v4) == pref_list.cend());
    }

    SECTION("find_index") {
        REQUIRE(pref_list.find_index(v1) == 0);
        REQUIRE(pref_list.find_index(v2) == 1);
        REQUIRE(pref_list.find_index(v3) == 2);

        REQUIRE(pref_list.find_index(v4) == 3);
    }
}

TEST_CASE("PreferenceList get", "[pref_list]") {
    PreferenceList pref_list;
    std::shared_ptr<Vertex> v1 = std::make_shared<Vertex>("v1");
    std::shared_ptr<Vertex> v2 = std::make_shared<Vertex>("v2");
    std::shared_ptr<Vertex> v3 = std::make_shared<Vertex>("v3");

    pref_list.emplace_back(v1);
    pref_list.emplace_back(v2);
    pref_list.emplace_back(v3);

    REQUIRE(pref_list.at(0).vertex == v1);
    REQUIRE(pref_list.at(1).vertex == v2);
    REQUIRE(pref_list.at(2).vertex == v3);

    REQUIRE_THROWS(pref_list.at(4));
}
