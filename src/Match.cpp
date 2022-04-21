/*
    thue, an interpreter for the Thue metalanguage
    Copyright (C) 2021  masterbuilder

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Match.h"
#include <iostream>

/***************
 * CONSTRUCTORS *
 ***************/
// TODO scan from both ends simultaneously
Match::Match(const Pattern& pattern, const std::string_view string, const bool submatch)
    : pattern(pattern), string(string), submatch(submatch)
{
  enum
  {
    START,
    CHARACTER,
    LITERAL,
    RECURSION,
    STRING,
    END
  };

  for (const auto& alternative : pattern.alternatives()) {
    std::vector<Constituent> constituents;
    int                      minimum_length = 0;

    for (const auto& constituent : alternative) {
      std::visit([&constituents](const auto& constituent) { constituents.push_back(Constituent(constituent)); },
                 constituent);

      switch (constituent.index()) {
        case CHARACTER:
          minimum_length++;
          break;
        case LITERAL:
          minimum_length += std::get<LITERAL>(constituent).value.size();
          break;
      }
    }

    auto& end            = constituents[constituents.size() - 1];

    end.possible_indices = { string.size(), string.size() };

    alternatives.push_back(constituents);

    minimum_lengths.push_back(minimum_length);
  }
}

/**************************
 * PUBLIC MEMBER FUNCTIONS *
 **************************/
bool Match::match()
{
  for (auto& alternative : alternatives) {
    reset();

    try {
      if (locate_anchor(alternative)) fixed_point_deduce(alternative, true);
      else fixed_point_deduce(alternative, false);
    }
    catch (const Contradiction& c) {
      match_index++;
      std::cout << "This alternative wasn't a match, moving on\n";
      continue;
    }

    // TODO don't use operator bool()
    if (!*this) {
      match_index++;
      continue;
    }

    return maybe_recurse(alternative);
  }

  return false;
}

IndexPair Match::match_indices() const
{
  const auto& start = alternatives[match_index][1];
  const auto& end   = alternatives[match_index][alternatives[match_index].size() - 2];

  return { start.possible_indices.first, end.possible_indices.second };
}

/***********************
 * CONVERSION OPERATORS *
 ***********************/
// TODO check if recursive matches fill up the intermediate space
Match::operator bool()
{
  auto&       alternative = alternatives[match_index];
  const auto& start       = alternative[1];
  const auto& end         = alternative[alternative.size() - 2];
  const auto  first_index = start.possible_indices.first;
  const auto  last_index  = end.possible_indices.second;

  if (submatch && (first_index != 0 || last_index != string.size())) return false;
  else if ((last_index - first_index) < minimum_lengths[match_index]) return false;

  for (auto c1 = alternative.begin() + 1, c2 = alternative.begin() + 2; c2 != alternative.end() - 1; c1++, c2++) {
    Constituent& first_constituent      = *c1;
    Constituent& second_constituent     = *c2;

    const auto [left_start, left_end]   = first_constituent.possible_indices;
    const auto [right_start, right_end] = second_constituent.possible_indices;

    if (first_constituent.matched == false) return false;
    else if (left_end != right_start) return false;
  }

  return true;
}

/***************************
 * PRIVATE MEMBER FUNCTIONS *
 ***************************/
bool Match::deduce(Constituent& c1, Constituent& c2, bool anchored)
{
  switch (c1.type) {
    case Constituent::Type::CHARACTER:
      switch (c2.type) {
        case Constituent::Type::CHARACTER:
          deduce_character_and_character(c1, c2);
          break;
        case Constituent::Type::STRING:
          deduce_character_and_string(c1, c2);
          break;
        case Constituent::Type::LITERAL:
          deduce_character_and_literal(c1, c2);
          break;
        case Constituent::Type::END:
          if (!anchored) deduce_character_on_left(c1, c2);
          break;
        case Constituent::Type::RECURSION:
          deduce_recursion_on_right(c1, c2);
          break;
        default:
          break;
      }
      break;
    case Constituent::Type::STRING:
      switch (c2.type) {
        case Constituent::Type::CHARACTER:
          deduce_string_and_character(c1, c2);
          break;
        case Constituent::Type::LITERAL:
          if (anchored) deduce_string_and_literal(c1, c2);
          break;
        case Constituent::Type::END:
          deduce_string_on_left(c1, c2);
          break;
        default:
          break;
      }
      break;
    case Constituent::Type::LITERAL:
      switch (c2.type) {
        case Constituent::Type::CHARACTER:
          deduce_literal_and_character(c1, c2);
          break;
        case Constituent::Type::STRING:
          if (anchored) deduce_literal_and_string(c1, c2);
          break;
        case Constituent::Type::RECURSION:
          deduce_recursion_on_right(c1, c2);
          break;
        default:
          break;
      }
      break;
    case Constituent::Type::START:
      switch (c2.type) {
        case Constituent::Type::STRING:
          deduce_string_on_right(c1, c2);
          break;
        case Constituent::Type::CHARACTER:
          if (!anchored) deduce_character_on_right(c1, c2);
          break;
        case Constituent::Type::END:
          if (!anchored) return false;
          break;
        default:
          break;
      }
      break;
    case Constituent::Type::RECURSION:
      switch (c2.type) {
        case Constituent::Type::CHARACTER:
        case Constituent::Type::LITERAL:
          deduce_recursion_on_left(c1, c2);
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }

  return c1.matched && c2.matched;
}

void Match::fixed_point_deduce(auto& alternative, bool anchored)
{
  bool changing                 = false;
  int  last_number_of_unmatched = 0;
  bool matched                  = false;

  do {
    index                   = 0;
    int number_of_unmatched = 0;

    for (auto c1 = alternative.begin(), c2 = alternative.begin() + 1; c2 != alternative.end(); c1++, c2++) {
      Constituent& first_constituent  = *c1;
      Constituent& second_constituent = *c2;

      bool deduced                    = anchored ? deduce(first_constituent, second_constituent, true)
                                                 : deduce(first_constituent, second_constituent, false);

      if (!deduced) {
        matched = false;
        number_of_unmatched++;
      }
    }

    if (matched) break;

    if (last_number_of_unmatched != number_of_unmatched) {
      changing                 = true;
      last_number_of_unmatched = number_of_unmatched;
    }
    else changing = false;
  } while (changing);
}

void Match::check_for_contradiction(const Constituent& c)
{
  if (!pattern.map.contains(c.id)) pattern.map[c.id] = c.deduced_value;
  else if (pattern.map[c.id] != c.deduced_value) throw Contradiction(c);
}

void Match::deduce_recursion_on_left(Constituent& r, Constituent& c2)
{
  if (r.matched || !c2.matched) return;

  const auto [left_start, left_end]   = r.possible_indices;
  const auto [right_start, right_end] = c2.possible_indices;

  r.possible_indices                  = { left_start, right_start };

  r.matched                           = true;
  recursive                           = true;

  if (right_start - left_start == 0) {
    recursive = false;
  }
  else if (right_start != std::string::npos) {
    std::cout << "Deduced recursion\n";
  }
  else {
    r.matched = false;
    recursive = false;
  }
}

void Match::deduce_recursion_on_right(Constituent& c1, Constituent& r)
{
  if (r.matched || !c1.matched) return;

  const auto [left_start, left_end]   = c1.possible_indices;
  const auto [right_start, right_end] = r.possible_indices;

  r.possible_indices                  = { left_end, right_end };

  r.matched                           = true;
  recursive                           = true;

  if (right_end - left_end == 0) {
    recursive = false;
  }
  else if (right_end != std::string::npos) {
    std::cout << "Deduced recursion\n";
  }
  else {
    r.matched = false;
    recursive = false;
  }
}

bool Match::deduce_literal(Constituent& l)
{
  if (l.type == Constituent::Type::RECURSION) recursive = true;

  if (l.type != Constituent::Type::LITERAL) return false;

  const auto [start, end] = find_start_and_end_of_literal(l);

  if (start == std::string::npos) return false;

  l.possible_indices = { start, end };
  l.matched          = true;

  index              = end;

  return true;
}

void Match::deduce_character_and_character(Constituent& c1, Constituent& c2)
{
  if (!c1.matched && c2.matched) deduce_character_on_left(c1, c2);
  else if (c1.matched && !c2.matched) deduce_character_on_right(c1, c2);

  index++;
}

void Match::deduce_character_and_string(Constituent& c, Constituent& s)
{
  if (!c.matched && s.matched) deduce_character_on_left(c, s);
  else if (c.matched && !s.matched) deduce_string_on_right(c, s);

  index++;
}

void Match::deduce_character_and_literal(Constituent& c, Constituent& l)
{
  if (c.matched && l.matched) return;
  else if (!l.matched) {
    const auto [start, end] = find_start_and_end_of_literal(l);

    if (start == std::string::npos) return;

    l.possible_indices = { start, end };
    l.matched          = true;
  }

  deduce_character_on_left(c, l);

  index++;
}

void Match::deduce_character_on_left(Constituent& c1, Constituent& c2)
{
  if (c1.matched) return;

  const auto [left_start, left_end]   = c2.possible_indices;

  c1.possible_indices                 = { left_start - 1, left_start };

  const auto [right_start, right_end] = c1.possible_indices;

  c1.deduced_value                    = string[right_start];

  check_for_contradiction(c1);

  c1.matched = true;

  std::cout << "Deduced character c" << c1.id << " to be " << c1.deduced_value << '\n';
}

void Match::deduce_character_on_right(Constituent& c1, Constituent& c2)
{
  if (c2.matched) return;

  const auto [left_start, left_end]   = c1.possible_indices;

  c2.possible_indices                 = { left_end, left_end + 1 };

  const auto [right_start, right_end] = c2.possible_indices;

  c2.deduced_value                    = string[right_start];

  check_for_contradiction(c2);

  c2.matched = true;

  std::cout << "Deduced character c" << c2.id << " to be " << c2.deduced_value << '\n';
}

void Match::deduce_string_and_character(Constituent& s, Constituent& c)
{
  if (s.matched && c.matched) return;
  else if (s.matched) deduce_character_on_right(s, c);
  else if (c.matched) deduce_string_on_left(s, c);

  index++;
}

void Match::deduce_string_and_literal(Constituent& s, Constituent& l)
{
  if (!l.matched) {
    const auto [start, end] = find_start_and_end_of_literal(l);

    if (start == std::string::npos) return;

    l.possible_indices = { start, end };
    l.matched          = true;

    deduce_string_on_left(s, l);

    index = end;
  }
  else deduce_string_on_left(s, l);
}

void Match::deduce_string_on_left(Constituent& s, Constituent& c2)
{
  if (s.matched) return;

  const auto [left_start, left_end]   = s.possible_indices;
  const auto [right_start, right_end] = c2.possible_indices;

  s.possible_indices                  = { left_start, right_start };

  if (left_start != std::string::npos) {
    s.deduced_value = string.substr(left_start, right_start - left_start);

    check_for_contradiction(s);

    s.matched = true;

    std::cout << "Deduced string s" << s.id << " to be " << s.deduced_value << '\n';
  }
}

void Match::deduce_string_on_right(Constituent& c1, Constituent& s)
{
  if (s.matched) return;

  const auto [left_start, left_end]   = c1.possible_indices;
  const auto [right_start, right_end] = s.possible_indices;

  s.possible_indices                  = { left_end, right_end };

  if (right_end != std::string::npos) {
    s.deduced_value = string.substr(left_end, right_end - left_end);

    check_for_contradiction(s);

    s.matched = true;

    std::cout << "Deduced string s" << s.id << " to be " << s.deduced_value << '\n';
  }
}

void Match::deduce_literal_and_character(Constituent& l, Constituent& c)
{
  if (l.matched && c.matched) return;

  if (!l.matched) {
    const auto [start, end] = find_start_and_end_of_literal(l);

    if (start == std::string::npos) return;

    l.possible_indices = { start, end };
    l.matched          = true;

    index              = end;
  }

  deduce_character_on_right(l, c);
}

void Match::deduce_literal_and_string(Constituent& l, Constituent& s)
{
  if (!l.matched) {
    const auto [start, end] = find_start_and_end_of_literal(l);

    if (start == std::string::npos) return;

    l.possible_indices = { start, end };
    l.matched          = true;

    index              = end;
  }

  deduce_string_on_right(l, s);
}

std::pair<std::size_t, std::size_t> Match::find_start_and_end_of_literal(const Constituent& l)
{
  const std::string_view literal = l.deduced_value;

  const std::size_t start        = string.find(literal, index);

  std::size_t end                = start + literal.size();
  if (end >= string.size()) end = string.size();

  return { start, end };
}

bool Match::locate_anchor(auto& alternative)
{
  bool contains_anchor = false;

  for (auto& constituent : alternative)
    if (deduce_literal(constituent)) contains_anchor = true;

  return contains_anchor;
}

IndexPair Match::match(const Constituent c) { return match(c.deduced_value); }

IndexPair Match::match(const std::string_view literal)
{
  std::size_t index_of_match = string.find(literal);

  if (index_of_match == std::string::npos) return { std::string::npos, std::string::npos };
  else return { index_of_match, index_of_match + literal.size() };
}

bool Match::maybe_recurse(auto& alternative)
{
  bool matched = true;

  if (recursive) {
    matched = false;

    for (auto& constituent : alternative)
      if (constituent.type == Constituent::Type::RECURSION && constituent.matched) {
        const auto [start, end]          = constituent.possible_indices;

        const std::string_view substring = string.substr(start, end - start);

        Match match(pattern, substring, true);

        matched = match.match();

        // if (matched) std::cout << "Recursively matched\n";
      }
  }

  return matched;
}

// TODO when should only temporary indices be reset?
void Match::reset()
{
  recursive = false;

  pattern.map.clear();
  // std::erase_if(pattern.map, [](const auto& element) {
  //   const auto& [k, _] = element;

  //   return k < 0;
  // });
}
