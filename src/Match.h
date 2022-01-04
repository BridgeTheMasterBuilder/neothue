#ifndef MATCH_H
#define MATCH_H

#include "Grammar.h"
#include <string_view>
#include <vector>

// TODO store the deduced values in this type so as to support back references?
class Match {
public:
  Match(const Alternative& alternative, const std::string_view string);
  bool      is_match() const;
  IndexPair match_indices() const;

private:
  IndexPair find_possible_indices(std::vector<IndexPair>& available_indices, std::size_t index);
  IndexPair match(const Character& c);
  IndexPair match(const String& s);
  IndexPair match(const Literal& l);
  // TODO update available?
  void      remove_indices(std::vector<IndexPair>& available_indices, IndexPair match);

private:
  // TODO vector of Constituents that each keep track of their possible values
  std::vector<IndexPair> available_indices;
  const std::string_view string;
  std::size_t            index = 0;
};

#endif
