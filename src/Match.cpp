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
// TODO recursion doesn't quite work
Match::Match(const Alternative& alternative, const std::string& string) : string(string)
{
  for (const auto& constituent : alternative)
    std::visit([this](const auto& constituent) { return constituents.push_back(Constituent(constituent)); },
               constituent);

  constituents[constituents.size() - 1].possible_indices = { string.size(), string.size() };

  bool contains_anchor                                   = false;

  for (auto& constituent : constituents)
    if (deduce_literal(constituent)) contains_anchor = true;

  try {
    if (contains_anchor) fixed_point_anchored_deduce();
    else fixed_point_unanchored_deduce();
  }
  catch (const Contradiction& c) {
    ;
  }

  if (recursive) {
    for (auto& constituent : constituents)
      if (constituent.type == Constituent::Type::RECURSION) {
        const auto [start, end]     = constituent.possible_indices;

        const std::string substring = string.substr(start, end - start);

        Match match(alternative, substring);

        if (match) std::cout << "Recursively matched\n";
      }
  }
}

/**************************
 * PUBLIC MEMBER FUNCTIONS *
 **************************/
IndexPair Match::match_indices() const
{
  return { constituents[1].possible_indices.first, constituents[constituents.size() - 2].possible_indices.second };
}

/***********************
 * CONVERSION OPERATORS *
 ***********************/
Match::operator bool() const
{
  for (const auto& constituent : constituents)
    if (constituent.matched == false) return false;

  return true;
}

/***************************
 * PRIVATE MEMBER FUNCTIONS *
 ***************************/
bool Match::anchored_deduce(Constituent& c1, Constituent& c2)
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
          deduce_string_and_literal(c1, c2);
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
          deduce_literal_and_string(c1, c2);
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

void Match::deduce_recursion_on_left(Constituent& r, Constituent& c2)
{
  if (r.matched) return;

  const auto [left_start, left_end]   = r.possible_indices;
  const auto [right_start, right_end] = c2.possible_indices;

  r.possible_indices                  = { left_start, right_start };

  if (right_start - left_start == 0) recursive = false;
  else if (right_start != std::string::npos) {
    r.matched = true;

    std::cout << "Deduced recursion\n";
  }
}

void Match::deduce_recursion_on_right(Constituent& c1, Constituent& r)
{
  if (r.matched) return;

  const auto [left_start, left_end]   = c1.possible_indices;
  const auto [right_start, right_end] = r.possible_indices;

  r.possible_indices                  = { left_end, right_end };

  if (right_end - left_end == 0) recursive = false;
  else if (right_end != std::string::npos) {
    r.matched = true;

    std::cout << "Deduced recursion\n";
  }
}

void Match::check_for_contradiction(const Constituent& c)
{
  if (!string_map.contains(c.id)) string_map[c.id] = c.deduced_value;
  else if (string_map[c.id] != c.deduced_value) throw Contradiction(c);
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

// void Match::deduce_string_and_string([[maybe_unused]] Constituent& c1, [[maybe_unused]] Constituent& c2) { return; }

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

void Match::fixed_point_anchored_deduce()
{
  bool changing                 = false;
  int  last_number_of_unmatched = 0;
  bool matched                  = false;

  do {
    index                   = 0;
    int number_of_unmatched = 0;

    for (auto c1 = constituents.begin(), c2 = constituents.begin() + 1; c2 != constituents.end(); c1++, c2++) {
      Constituent& first_constituent  = *c1;
      Constituent& second_constituent = *c2;

      if (!anchored_deduce(first_constituent, second_constituent)) {
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

void Match::fixed_point_unanchored_deduce()
{
  bool changing                 = false;
  int  last_number_of_unmatched = 0;
  bool matched                  = false;

  do {
    index                   = 0;
    int number_of_unmatched = 0;

    for (auto c1 = constituents.begin(), c2 = constituents.begin() + 1; c2 != constituents.end(); c1++, c2++) {
      Constituent& first_constituent  = *c1;
      Constituent& second_constituent = *c2;

      if (!unanchored_deduce(first_constituent, second_constituent)) {
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

IndexPair Match::match(const Constituent c) { return match(c.deduced_value); }

IndexPair Match::match(const std::string_view literal)
{
  std::size_t index_of_match = string.find(literal);

  if (index_of_match == std::string::npos) return { std::string::npos, std::string::npos };
  else return { index_of_match, index_of_match + literal.size() };
}

bool Match::unanchored_deduce(Constituent& c1, Constituent& c2)
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
          deduce_character_on_left(c1, c2);
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
          deduce_character_on_right(c1, c2);
          break;
        case Constituent::Type::END:
          return false;
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
