#ifndef T_DEFS_H
#define T_DEFS_H

#include <memory>

// forward declaration for the Vertex class
class Vertex;

/// Id type for a vertex
typedef std::string IdType;

// Rank representation for a vertex in the preference/partner list
// this must be unique for all the vertex in a list
typedef int RankType;

// Pointer type for vertices
typedef std::shared_ptr<Vertex> VertexPtr;

#endif
