#include "Match.h"
#include <iostream>

Match::Match(const Alternative& alternative, const std::string_view string) : string(string)
{
  // TODO naming
  for (auto& constituent : alternative) {
    Constituent c           = std::visit([](const auto& constituent) { return Constituent(constituent); }, constituent);

    const auto [start, end] = match(c);

    if (start == std::string::npos) break;

    c.matched = true;
  }
}

// IndexPair Match::find_possible_indices(std::vector<IndexPair>& available_indices, std::size_t index)
// {
//   for (const auto& pair : available_indices) {
//     const auto& [start, end] = pair;

//     if (index < start) continue;
//     if (index < end) return pair;
//   }

//   return { std::string::npos, std::string::npos };
// }

Match::operator bool() const
{
  for (const auto& constituent : constituents)
    if (constituent.matched == false) return false;

  return true;
}

IndexPair Match::match(Constituent c)
{
  // if (c.type == Constituent::Type::CHARACTER) return match_character();
  // else if (c.type == Constituent::Type::STRING) return match_string();
  // else return match_literal(c.deduced_value);
  return match_literal(c.deduced_value);
}

// // TODO update available
// IndexPair Match::match_character() { return find_possible_indices(available_indices, index++); }

IndexPair Match::match_literal(const std::string_view literal)
{
  // IndexPair match = { start, end };
  // remove_indices(available_indices, match);
  // index = end;

  std::size_t index_of_match = string.find(literal);

  if (index_of_match == std::string::npos) return { std::string::npos, std::string::npos };
  else return { index_of_match, index_of_match + literal.size() };
}

// // TODO update available
// IndexPair Match::match_string() { return find_possible_indices(available_indices, index); }

// void Match::update_possible_indices(std::vector<IndexPair>& available_indices, IndexPair match)
// {
//   const auto [match_start, match_end] = match;

//   // std::cout << "Available indices before removal:\n";
//   // for (const auto& [start, end] : available_indices)
//   //   std::cout << '[' << start << ", " << end << ")\n";

//   for (std::size_t i = 0; i < available_indices.size(); i++) {
//     auto& [start, end] = available_indices[i];

//     // overlap only on the left side
//     if (match_start < start && start < match_end) start = match_end;
//     // overlap only on the right side
//     else if (match_end > end && end > match_start) end = match_start;
//     // contained within => partition
//     else if (match_start >= start && match_end <= end) {
//       std::size_t old_end = end;
//       end                 = match_start;
//       if (end <= start) {
//         available_indices.erase(available_indices.begin() + i);
//         available_indices.emplace(available_indices.begin() + i, match_end, old_end);
//       }
//       else available_indices.emplace(available_indices.begin() + i + 1, match_end, old_end);
//     }
//   }

//   // std::cout << "Available indices after removal\n";
//   // for (const auto& [start, end] : available_indices)
//   //   std::cout << '[' << start << ", " << end << ")\n";
// }
