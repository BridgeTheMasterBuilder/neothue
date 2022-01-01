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

#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "concepts.h"
#include <random>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using Production = std::pair<std::string, std::string>;

enum class application_order
{
  LEFT_TO_RIGHT,
  RIGHT_TO_LEFT,
  NONDETERMINISTIC
};

class Grammar {
public:
  // CONSTRUCTORS
  Grammar(const application_order order = application_order::NONDETERMINISTIC, const bool classic = false, const bool debug = false);

  // PUBLIC MEMBER FUNCTIONS
  void add_production(const std::string_view lhs, const std::string_view rhs);
  void apply_productions(std::string& initial_state);
  void sort();

private:
  // PRIVATE MEMBER FUNCTIONS
  void apply_production(Production& production, std::string& string, const std::size_t index_of_match);
  template<typename T>
  std::size_t match(const T lhs, const std::string_view string) requires(any_of<T, const char*, std::string_view, char>)
  {
    using enum application_order;

    std::size_t index_of_match;

    if (order == LEFT_TO_RIGHT) index_of_match = string.find(lhs);
    else if (order == RIGHT_TO_LEFT) index_of_match = string.rfind(lhs);
    else index_of_match = (rng() < rng.max() / 2) ? string.find(lhs) : string.rfind(lhs);

    return index_of_match;
  }
  std::size_t match(const std::string& lhs, const std::string_view string)
  {
    return match<std::string_view>(lhs, string);
  }
  void rewrite_production(Production& production, const std::string_view rhs);
  void shuffle();
  void sort_left_to_right();
  void sort_right_to_left();

  // PRIVATE DATA
  std::vector<Production> productions;
  std::mt19937            rng;
  const application_order order;
  const bool              classic;
  const bool              debug;
};

#endif
