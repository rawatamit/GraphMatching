#include "catch.hpp"
#include "GraphReader.h"
#include "Utils.h"
#include "TestDefs.h"
#include <memory>

TEST_CASE("GraphReader missing_pref_partition_A", "[reader]") {
    std::string pathA(get_filepath(get_resources_dir(), "parser/missing_pref_partition_A.txt"));
    CHECK_THROWS_AS(read_graph(pathA), ReaderException);

    std::string pathB(get_filepath(get_resources_dir(), "parser/missing_pref_partition_B.txt"));
    CHECK_THROWS_AS(read_graph(pathB), ReaderException);
}

TEST_CASE("GraphReader duplicate_partition", "[reader]") {
    std::string pathA(get_filepath(get_resources_dir(), "parser/duplicate_partition_A.txt"));
    CHECK_THROWS_AS(read_graph(pathA), ReaderException);

    std::string pathB(get_filepath(get_resources_dir(), "parser/duplicate_partition_B.txt"));
    CHECK_THROWS_AS(read_graph(pathB), ReaderException);
}

TEST_CASE("GraphReader duplicate_preference_lists", "[reader]") {
    std::string pathA(get_filepath(get_resources_dir(), "parser/duplicate_preflists_A.txt"));
    CHECK_THROWS_AS(read_graph(pathA), ReaderException);

    std::string pathB(get_filepath(get_resources_dir(), "parser/duplicate_preflists_B.txt"));
    CHECK_THROWS_AS(read_graph(pathB), ReaderException);
}

TEST_CASE("GraphReader missing_partition", "[reader]") {
    std::string pathA(get_filepath(get_resources_dir(), "parser/missing_partition_A.txt"));
    CHECK_THROWS_AS(read_graph(pathA), ReaderException);

    std::string pathB(get_filepath(get_resources_dir(), "parser/missing_partition_B.txt"));
    CHECK_THROWS_AS(read_graph(pathB), ReaderException);
}

TEST_CASE("GraphReader missing_preference_lists", "[reader]") {
    std::string pathA(get_filepath(get_resources_dir(), "parser/missing_preflists_A.txt"));
    CHECK_THROWS_AS(read_graph(pathA), ReaderException);

    std::string pathB(get_filepath(get_resources_dir(), "parser/missing_preflists_B.txt"));
    CHECK_THROWS_AS(read_graph(pathB), ReaderException);
}
