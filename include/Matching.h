#ifndef MATCHING_H
#define MATCHING_H

#include "PartnerList.h"
#include "TDefs.h"
#include <map>
#include <vector>

class Matching {
public:
  typedef std::map<VertexPtr, PartnerList> MatchedPairListType;

public:
  Matching() {}
  virtual ~Matching() {}

  void add_partner(VertexPtr u, const Partner &v, int level);
  void add_partner(VertexPtr u, VertexPtr v, RankType rank, int level);

  void remove_partners(VertexPtr u, VertexPtr v);

  // get partners for v in M
  const PartnerList& get_partners(VertexPtr v) const;

  // # of partners for v in M
  PartnerList::SizeType number_of_partners(VertexPtr v) const;

  // does v have at least one partner in M
  bool has_partner(VertexPtr v) const;

  // Does v have these partners.
  bool has_partners(VertexPtr v, const std::vector<VertexPtr>& partners) const;

  // get matched partner for v in M
  VertexPtr get_partner(VertexPtr v) const;

  // Is u matched to v^level
  bool is_matched_to(VertexPtr u, VertexPtr v, int level) const;

  // size of matching M
  int size() const;

  bool empty() const;

  const MatchedPairListType& get_matched_pairs() const;

private:
  MatchedPairListType _matchedPairs;
  // When a vertex has no matched partners, an empty partner list is returned.
  PartnerList _emptyPartnerList;
};

#endif // MATCHING_H
