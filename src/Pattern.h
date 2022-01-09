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

#ifndef PATTERN_H
#define PATTERN_H

#include <string>
#include <string_view>
#include <variant>
#include <vector>

// TODO use variant instead of classes?
struct Character {
  Character(const int value) : value(value) { }

  operator int() { return value; }

  const int value = 0;
};

struct String {
  String(const int value) : value(value) { }

  operator int() { return value; }

  const int value = 0;
};

struct Literal {
  Literal(const std::string_view value) : value(value) { }

  operator std::string_view() { return value; }

  const std::string value;
};

struct Start {
};

struct End {
};

using Alternative = std::vector<std::variant<Start, Character, Literal, String, End>>;

class Pattern {
public:
  // PUBLIC MEMBER FUNCTIONS
  void        add_alternative(const std::string_view pattern);
  const auto& alternatives() const { return _alternatives; }

  // FRIEND FUNCTIONS
  friend bool operator<(const Pattern& a, const Pattern& b) { return a.representative < b.representative; }

private:
  // PRIVATE MEMBER FUNCTIONS
  Alternative                         analyze_pattern(const std::string& pattern);
  std::pair<int, std::size_t>         parse_id(const std::string& pattern, const std::size_t index);
  std::pair<std::string, std::size_t> parse_literal(const std::string& pattern, const std::size_t index);

  // PRIVATE DATA
  std::string_view         representative;
  std::vector<Alternative> _alternatives;
};

#endif
