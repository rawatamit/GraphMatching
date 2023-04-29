#include "Matching.h"
#include "Utils.h"
#include <cassert>
#include <algorithm>

void Matching::add_partner(VertexPtr u, const Partner &v, int level) {
  _matchedPairs[u].add_partner(v.vertex, v.rank, level);
}

void Matching::add_partner(VertexPtr u, VertexPtr v, RankType rank, int level) {
  _matchedPairs[u].add_partner(v, rank, level);
}

void Matching::remove_partners(VertexPtr u, VertexPtr v) {
  _matchedPairs.at(u).remove(v);
  _matchedPairs.at(v).remove(u);
}

const PartnerList& Matching::get_partners(VertexPtr v) const {
  auto v_it = _matchedPairs.find(v);
  return (v_it == _matchedPairs.end()) ? _emptyPartnerList : v_it->second;
}

PartnerList::SizeType Matching::number_of_partners(VertexPtr v) const {
  return get_partners(v).size();
}

bool Matching::has_partner(VertexPtr v) const {
  return number_of_partners(v) > 0;
}

bool Matching::has_partners(VertexPtr v, const std::vector<VertexPtr>& partners) const {
    const auto& M_v = get_partners(v);

    for (auto it = M_v.cbegin(); it != M_v.cend(); ++it) {
        if (std::find(partners.cbegin(), partners.cend(), it->vertex) == partners.cend()) {
            return false;
        }
    }

    return M_v.size() == partners.size();
}

VertexPtr Matching::get_partner(VertexPtr v) const {
  const PartnerList &partners(get_partners(v));
  assert(partners.size() == 1);
  return partners.get_partner();
}

bool Matching::is_matched_to(VertexPtr u, VertexPtr v, int level) const {
  const auto &partners = get_partners(u);
  const auto &partner_it = partners.find(v);
  // If partner is present, check level matched to this partner is equal.
  return (partner_it != partners.cend()) ? (partner_it->level == level) : false;
}

int Matching::size() const {
  int size = 0;
  for (const auto &it : _matchedPairs) {
    size += it.second.size();
  }
  return size / 2;
}

bool Matching::empty() const
{
  return size() == 0;
}

const Matching::MatchedPairListType& Matching::get_matched_pairs() const
{
  return _matchedPairs;
}
