/*
    nthue, an interpreter for Neothue, a dialect of the Thue metalanguage
    Copyright (C) 2022  masterbuilder

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
#include <algorithm>
#include <cstddef>
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
  Grammar(const application_order order   = application_order::NONDETERMINISTIC,
          const bool              classic = false,
          const bool              debug   = false);

  // PUBLIC MEMBER FUNCTIONS
  void add_production(const std::string_view lhs, const std::string_view rhs);
  void apply_productions(std::string& initial_state);
  void sort();

private:
  // PRIVATE MEMBER FUNCTIONS
  void apply_production(Production& production, std::string& string, const std::size_t index_of_match);
  template<typename T>
  std::size_t match(const T lhs, const std::string_view string)
    requires(any_of<T, const char*, std::string_view, char>)
  {
    using enum application_order;

    std::size_t              index_of_match = 0;
    std::vector<std::size_t> indices;

    while (true) {
      index_of_match = string.find(lhs, index_of_match);

      if (index_of_match == std::string::npos) break;

      indices.push_back(index_of_match);
      index_of_match++;
    }

    if (indices.empty()) return std::string::npos;

    std::size_t random_index = rng() % indices.size();

    return indices[random_index];
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
  // TODO don't forget to make private
public:
  std::vector<Production> productions;

private:
  std::vector<std::size_t> indices;
  std::mt19937             rng;
  application_order        order;
  bool                     classic;
  bool                     debug;
};

#endif
