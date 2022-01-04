#include "Match.h"
#include <iostream>

bool Match::attempt_to_match(const auto& constituent)
{
  enum
  {
    CHARACTER,
    STRING,
    LITERAL
  };

  if (constituent.index() == LITERAL) {
    const auto [start, end] = match(std::get<LITERAL>(constituent).value, string);

    if (start == std::string::npos) break;

    std::cout << "Found literal " << std::get<LITERAL>(constituent).value << " at indices " << start << "-" << end
              << "\n";

    IndexPair match = { start, end };
    remove_indices(available_indices, match);
    index = end;
  }
  else if (constituent.index() == CHARACTER) {
    const auto [start, end] = find_possible_indices(available_indices, index);

    if (start == std::string::npos) break;

    if (end == start + 1) {
      std::cout << "Character with id " << std::get<CHARACTER>(constituent).value << " is located at position " << start
                << '\n';
    }
    else {
      std::cout << "Character with id " << std::get<CHARACTER>(constituent).value << " may be located at indices "
                << start << "-" << end << "\n";
    }

    index++;
  }
  else {
    const auto [start, end] = find_possible_indices(available_indices, index);

    if (start == std::string::npos) break;

    std::cout << "String with id " << std::get<STRING>(constituent).value << " may be located at indices " << start
              << "-" << end << "\n";
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

void Match::remove_indices(std::vector<IndexPair>& available_indices, IndexPair match)
{
  const auto [match_start, match_end] = match;

  std::cout << "Available indices before removal:\n";
  for (const auto& [start, end] : available_indices)
    std::cout << '[' << start << ", " << end << ")\n";

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

  std::cout << "Available indices after removal\n";
  for (const auto& [start, end] : available_indices)
    std::cout << '[' << start << ", " << end << ")\n";
}
