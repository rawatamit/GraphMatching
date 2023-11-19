#ifndef MATCHING_H
#define MATCHING_H

#include "BipartiteGraph.h"
#include "PartnerList.h"
#include "TDefs.h"
#include <map>
#include <memory>
#include <set>
#include <vector>
#include <ostream>

class Matching {
public:
  typedef std::map<VertexPtr, PartnerList> MatchedPairListType;
  std::map<VertexPtr, std::vector<Partner>> uncertain_proposals;

public:
  Matching(bool isAProposing = true) : _isAProposing(isAProposing) {}
  virtual ~Matching() {}

  void add_partner(VertexPtr u, const Partner &v, int level);
  void add_partner(VertexPtr u, VertexPtr v, RankType rank, int level);
  void set_uncertain_proposal(VertexPtr u, VertexPtr v, RankType rank, int level);

  void remove_partner(VertexPtr u, VertexPtr v);

  // get partners for v in M
  const PartnerList &get_partners(VertexPtr v) const;

  // # of partners for v in M
  PartnerList::SizeType number_of_partners(VertexPtr v) const;

  // does v have at least one partner in M
  bool has_partner(VertexPtr v) const;

  // Does v have these partners.
  bool has_partners(VertexPtr v, const std::vector<VertexPtr> &partners) const;

  // Is b a part of an uncertain proposal
  bool check_uncertain_proposal(VertexPtr b) const;

  // get matched partner for v in M
  VertexPtr get_partner(VertexPtr v) const;

  // Is u matched to v?
  bool is_matched_to(VertexPtr u, VertexPtr v) const;

  // Is u matched to v^level?
  bool is_matched_to(VertexPtr u, VertexPtr v, int level) const;

  // Is u matched to any vertex at < level?
  bool is_matched_lt(VertexPtr u, int level) const;

  // Is u matched to v^x where x < level?
  bool is_matched_lt(VertexPtr u, VertexPtr v, int level) const;

  // size of matching M
  int size() const;

  bool empty() const;

  const MatchedPairListType &get_matched_pairs() const;

  // Use dual certificate to verify that this matching is popular.
  bool verify(std::shared_ptr<BipartiteGraph> G) const;

  friend std::ostream& operator<<(std::ostream& out, const Matching& M);

private:
  typedef std::pair<VertexPtr, int> VertexCopy;

  void generate_M_star(const BipartiteGraph::ContainerType &A_partition,
                       const BipartiteGraph::ContainerType &B_partition,
                       std::map<VertexCopy, VertexCopy> &M_star,
                       std::map<int, std::set<VertexCopy>> &A_levels,
                       std::map<int, std::set<VertexCopy>> &B_levels) const;

private:
  MatchedPairListType _matchedPairs;
  // When a vertex has no matched partners, an empty partner list is returned.
  PartnerList _emptyPartnerList;

  // FIXME: dual certificate verificate is only valid for popular matchings.
  // This verification should be either moved to MatchingAlgorithm class or
  // into a different abstraction. This variable should be removed as well.
  bool _isAProposing;
};

#endif // MATCHING_H
