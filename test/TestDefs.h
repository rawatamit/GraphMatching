#ifndef TESTDEFS_H
#define TESTDEFS_H

#include <string>
#include <vector>
#include <MatchingAlgorithm.h>

std::string get_root_dir();

std::string get_resources_dir();

std::string get_filepath(std::string dir, std::string filename);

// does v have exact partners in M
bool has_partners(std::shared_ptr<MatchingAlgorithm::MatchedPairListType> M, VertexPtr v, const std::vector<VertexPtr>& partners);

#endif // TESTDEFS_H
