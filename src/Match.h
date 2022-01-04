#ifndef MATCH_H
#define MATCH_H

#include "Grammar.h"
#include <string_view>
#include <vector>

class Match {
public:
  Match(const std::string_view string) : available_indices(1, { 0, string.size() }), string(string) { }
  bool      attempt_to_match(const auto& constituent);
  bool      is_match() const;
  IndexPair match_indices() const;

private:
  IndexPair find_possible_indices(std::vector<IndexPair>& available_indices, std::size_t index);
  void      remove_indices(std::vector<IndexPair>& available_indices, IndexPair match);

private:
  std::vector<IndexPair> available_indices;
  const std::string_view string;
  std::size_t            index = 0;
};

#endif
