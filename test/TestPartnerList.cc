#include "catch.hpp"
#include "Vertex.h"
#include "PartnerList.h"
#include <memory>

TEST_CASE("Partner <", "[partner]") {
    std::shared_ptr<Vertex> v1 = std::make_shared<Vertex>("v1");
    std::shared_ptr<Vertex> v2 = std::make_shared<Vertex>("v2");
    std::shared_ptr<Vertex> v3 = std::make_shared<Vertex>("v3");
    Partner p1(v1, 1, 0);
    Partner p2(v2, 2, 1);
    Partner p3(v3, 3, 2);

    SECTION("irreflexivity") {
        REQUIRE_FALSE(p1 < p1);
        REQUIRE_FALSE(p2 < p2);
        REQUIRE_FALSE(p3 < p3);
    }

    SECTION("asymmetry") {
        REQUIRE(p1 < p2);
        REQUIRE_FALSE(p2 < p1);

        REQUIRE(p1 < p3);
        REQUIRE_FALSE(p3 < p1);

        REQUIRE(p2 < p3);
        REQUIRE_FALSE(p3 < p2);
    }

    SECTION("transitivity") {
        REQUIRE(p1 < p2);
        REQUIRE(p2 < p3);
        REQUIRE(p1 < p3);
    }
}

TEST_CASE("PartnerList empty", "[partner_list]") {
    PartnerList partner_list;
    std::shared_ptr<Vertex> v1 = std::make_shared<Vertex>("v1");

    REQUIRE(partner_list.empty());

    SECTION("adding vertices to the partner_list") {
        partner_list.add_partner(v1, 1, 0);
        REQUIRE_FALSE(partner_list.empty());
    }
}

TEST_CASE("PartnerList find", "[partner_list]") {
    PartnerList partner_list;
    std::shared_ptr<Vertex> v1 = std::make_shared<Vertex>("v1");
    std::shared_ptr<Vertex> v2 = std::make_shared<Vertex>("v2");
    std::shared_ptr<Vertex> v3 = std::make_shared<Vertex>("v3");

    partner_list.add_partner(v1, 1, 0);
    partner_list.add_partner(v2, 2, 0);
    partner_list.add_partner(v3, 3, 0);

    REQUIRE(partner_list.find(v1)->vertex == v1);
    REQUIRE(partner_list.find(v2)->vertex == v2);
    REQUIRE(partner_list.find(v3)->vertex == v3);
}

TEST_CASE("PartnerList find_least_preferred", "[partner_list]") {
    PartnerList partner_list;
    std::shared_ptr<Vertex> v1 = std::make_shared<Vertex>("v1");
    std::shared_ptr<Vertex> v2 = std::make_shared<Vertex>("v2");
    std::shared_ptr<Vertex> v3 = std::make_shared<Vertex>("v3");

    SECTION("all same level vertices") {
        partner_list.add_partner(v1, 1, 0);
        partner_list.add_partner(v2, 2, 0);
        partner_list.add_partner(v3, 3, 0);

        REQUIRE(partner_list.get_least_preferred().vertex == v3);

        partner_list.remove_least_preferred();
        REQUIRE(partner_list.get_least_preferred().vertex == v2);

        partner_list.remove_least_preferred();
        REQUIRE(partner_list.get_least_preferred().vertex == v1);
    }

    SECTION("different level vertices") {
        partner_list.add_partner(v1, 1, 0);
        partner_list.add_partner(v2, 2, 1);
        partner_list.add_partner(v3, 3, 2);

        REQUIRE(partner_list.get_least_preferred().vertex == v1);

        partner_list.remove_least_preferred();
        REQUIRE(partner_list.get_least_preferred().vertex == v2);

        partner_list.remove_least_preferred();
        REQUIRE(partner_list.get_least_preferred().vertex == v3);
    }

    SECTION("add random") {
        partner_list.add_partner(v3, 3, 2);
        REQUIRE(partner_list.get_least_preferred().vertex == v3);

        partner_list.add_partner(v1, 1, 0);
        REQUIRE(partner_list.get_least_preferred().vertex == v1);

        partner_list.add_partner(v2, 2, 1);
        REQUIRE(partner_list.get_least_preferred().vertex == v1);
    }
}

TEST_CASE("PartnerList remove_least_preferred", "[partner_list]") {
    PartnerList partner_list;
    std::shared_ptr<Vertex> v1 = std::make_shared<Vertex>("v1");
    std::shared_ptr<Vertex> v2 = std::make_shared<Vertex>("v2");
    std::shared_ptr<Vertex> v3 = std::make_shared<Vertex>("v3");

    SECTION("all same level vertices") {
        partner_list.add_partner(v1, 1, 0);
        partner_list.add_partner(v2, 2, 0);
        partner_list.add_partner(v3, 3, 0);

        partner_list.remove_least_preferred();
        REQUIRE(partner_list.size() == 2);
        REQUIRE(partner_list.find(v3) == partner_list.cend());

        partner_list.remove_least_preferred();
        REQUIRE(partner_list.size() == 1);
        REQUIRE(partner_list.find(v2) == partner_list.cend());

        partner_list.remove_least_preferred();
        REQUIRE(partner_list.empty());
        REQUIRE(partner_list.find(v1) == partner_list.cend());

        REQUIRE(partner_list.empty());
    }

    SECTION("different level vertices") {
        partner_list.add_partner(v1, 1, 0);
        partner_list.add_partner(v2, 2, 1);
        partner_list.add_partner(v3, 3, 2);

        partner_list.remove_least_preferred();
        REQUIRE(partner_list.size() == 2);
        REQUIRE(partner_list.find(v1) == partner_list.cend());

        partner_list.remove_least_preferred();
        REQUIRE(partner_list.size() == 1);
        REQUIRE(partner_list.find(v2) == partner_list.cend());

        partner_list.remove_least_preferred();
        REQUIRE(partner_list.empty());
        REQUIRE(partner_list.find(v3) == partner_list.cend());

        REQUIRE(partner_list.empty());
    }
}

TEST_CASE("PartnerList remove", "[partner_list]") {
    PartnerList partner_list;
    std::shared_ptr<Vertex> v1 = std::make_shared<Vertex>("v1");
    std::shared_ptr<Vertex> v2 = std::make_shared<Vertex>("v2");
    std::shared_ptr<Vertex> v3 = std::make_shared<Vertex>("v3");

    // remove method should remove the vertex irrespective of its level
    SECTION("all same level vertices") {
        partner_list.add_partner(v1, 1, 0);
        partner_list.add_partner(v2, 2, 0);
        partner_list.add_partner(v3, 3, 0);

        partner_list.remove(v1);
        REQUIRE(partner_list.find(v1) == partner_list.cend());

        partner_list.remove(v2);
        REQUIRE(partner_list.find(v2) == partner_list.cend());

        partner_list.remove(v3);
        REQUIRE(partner_list.find(v3) == partner_list.cend());
    }

    SECTION("different level vertices") {
        partner_list.add_partner(v1, 1, 0);
        partner_list.add_partner(v2, 2, 1);
        partner_list.add_partner(v3, 3, 2);

        partner_list.remove(v1);
        REQUIRE(partner_list.find(v1) == partner_list.cend());

        partner_list.remove(v2);
        REQUIRE(partner_list.find(v2) == partner_list.cend());

        partner_list.remove(v3);
        REQUIRE(partner_list.find(v3) == partner_list.cend());
    }
}
