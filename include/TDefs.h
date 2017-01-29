#ifndef T_DEFS_H
#define T_DEFS_H

#include <map>
#include <memory>

// forward declaration
class Vertex;
class PartnerList;

/// Id type for a vertex
typedef std::string IdType;

// Rank representation for a vertex in the preference/partner list
// this must be unique for all the vertex in a list
typedef int RankType;

// Pointer type for vertices
typedef std::shared_ptr<Vertex> VertexPtr;

// Matched partners for every vertex
typedef std::map<std::shared_ptr<Vertex>, PartnerList> MatchedPairListType;

#endif
