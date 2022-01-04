#include "Match.h"
#include <iostream>

Match::Match(const Alternative& alternative, const std::string_view string)
    : available_indices(1, { 0, string.size() }), string(string)
{
  enum
  {
    CHARACTER,
    STRING,
    LITERAL
  };

  for (const auto& constituent : alternative) {
    const auto [start, end] = std::visit([this](const auto& constituent) { return match(constituent); }, constituent);

    if (start == std::string::npos) break;
  }
}

IndexPair Match::find_possible_indices(std::vector<IndexPair>& available_indices, std::size_t index)
{
  for (const auto& pair : available_indices) {
    const auto& [start, end] = pair;

    if (index < start) continue;
    if (index < end) return pair;
  }

  return { std::string::npos, std::string::npos };
}

// TODO update available
IndexPair Match::match([[maybe_unused]] const Character& c)
{
  return find_possible_indices(available_indices, index++);
}

// TODO update available
IndexPair Match::match([[maybe_unused]] const String& s) { return find_possible_indices(available_indices, index); }

IndexPair Match::match(const Literal& l)
{
  // IndexPair match = { start, end };
  // remove_indices(available_indices, match);
  // index = end;

  std::size_t index_of_match = string.find(l.value);

  if (index_of_match == std::string::npos) return { std::string::npos, std::string::npos };
  else return { index_of_match, index_of_match + l.value.size() };
}

void Match::remove_indices(std::vector<IndexPair>& available_indices, IndexPair match)
{
  const auto [match_start, match_end] = match;

  // std::cout << "Available indices before removal:\n";
  // for (const auto& [start, end] : available_indices)
  //   std::cout << '[' << start << ", " << end << ")\n";

  for (std::size_t i = 0; i < available_indices.size(); i++) {
    auto& [start, end] = available_indices[i];

    // overlap only on the left side
    if (match_start < start && start < match_end) start = match_end;
    // overlap only on the right side
    else if (match_end > end && end > match_start) end = match_start;
    // contained within => partition
    else if (match_start >= start && match_end <= end) {
      std::size_t old_end = end;
      end                 = match_start;
      if (end <= start) {
        available_indices.erase(available_indices.begin() + i);
        available_indices.emplace(available_indices.begin() + i, match_end, old_end);
      }
      else available_indices.emplace(available_indices.begin() + i + 1, match_end, old_end);
    }
  }

  // std::cout << "Available indices after removal\n";
  // for (const auto& [start, end] : available_indices)
  //   std::cout << '[' << start << ", " << end << ")\n";
}
