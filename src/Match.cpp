#include "Match.h"
#include <iostream>

Match::Match(const Alternative& alternative, const std::string_view string) : string(string)
{
  if (alternative.size() < 2) {
    Constituent constituent =
        std::visit([](const auto& constituent) { return Constituent(constituent); }, *alternative.begin());

    const auto [start, end] = match(constituent);

    if (start == std::string::npos) return;

    constituent.matched = true;
  }

  for (auto c1 = alternative.begin(), c2 = alternative.begin() + 1; c2 != alternative.end(); c1++, c2++) {
    Constituent first_constituent  = std::visit([](const auto& constituent) { return Constituent(constituent); }, *c1);
    Constituent second_constituent = std::visit([](const auto& constituent) { return Constituent(constituent); }, *c2);

    // TODO dispatch on types to deduce
  }
}

Match::operator bool() const
{
  for (const auto& constituent : constituents)
    if (constituent.matched == false) return false;

  return true;
}

IndexPair Match::match(const Constituent c) { return match(c.deduced_value); }

IndexPair Match::match(const std::string_view literal)
{
  std::size_t index_of_match = string.find(literal);

  if (index_of_match == std::string::npos) return { std::string::npos, std::string::npos };
  else return { index_of_match, index_of_match + literal.size() };
}
