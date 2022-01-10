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

#ifndef MATCH_H
#define MATCH_H

#include "Grammar.h"
#include <string_view>
#include <unordered_map>
#include <vector>

struct Constituent {
  // CONSTRUCTORS
  Constituent(const Character& c) : type(Type::CHARACTER), id(c.value) { }
  Constituent(const Literal& c) : type(Type::LITERAL), deduced_value(c.value) { }
  Constituent(const String& c) : type(Type::STRING), id(c.value) { }
  Constituent([[maybe_unused]] const Recursion& c) : type(Type::RECURSION), id(-1) { }
  Constituent([[maybe_unused]] const End& c) : type(Type::END), id(-1), matched(true) { }
  Constituent([[maybe_unused]] const Start& c)
      : type(Type::START), id(-1), possible_indices({ 0, 0 }), matched(true) { }

  // ENUMS
  enum class Type
  {
    START,
    CHARACTER,
    LITERAL,
    RECURSION,
    STRING,
    END
  } type;

  // PUBLIC DATA
  const int   id               = 0;
  IndexPair   possible_indices = { std::string::npos, std::string::npos };
  std::string deduced_value;
  bool        matched = false;
};

class Match {
public:
  // CONSTRUCTORS
  Match(const Alternative& alternative, const std::string& string);

  // EXCEPTIONS
  struct Contradiction {
    Contradiction(const Constituent& constituent) : constituent(constituent) { }

    const Constituent constituent;
  };

  // PUBLIC MEMBER FUNCTIONS
  IndexPair match_indices() const;

  // CONVERSION OPERATORS
  operator bool() const;

private:
  // PRIVATE MEMBER FUNCTIONS
  bool                                anchored_deduce(Constituent& c1, Constituent& c2);
  void                                check_for_contradiction(const Constituent& c);
  bool                                deduce(Constituent& c1, Constituent& c2);
  void                                deduce_character_and_character(Constituent& c1, Constituent& c2);
  void                                deduce_character_and_string(Constituent& c1, Constituent& c2);
  void                                deduce_character_and_literal(Constituent& c1, Constituent& c2);
  void                                deduce_character_on_left(Constituent& c1, Constituent& c2);
  void                                deduce_character_on_right(Constituent& c1, Constituent& c2);
  void                                deduce_string_and_character(Constituent& c1, Constituent& c2);
  // void                                deduce_string_and_string(Constituent& c1, Constituent& c2);
  void                                deduce_string_and_literal(Constituent& c1, Constituent& c2);
  void                                deduce_string_on_left(Constituent& c1, Constituent& c2);
  void                                deduce_string_on_right(Constituent& c1, Constituent& c2);
  bool                                deduce_literal(Constituent& l);
  void                                deduce_literal_and_character(Constituent& c1, Constituent& c2);
  void                                deduce_literal_and_string(Constituent& c1, Constituent& c2);
  std::pair<std::size_t, std::size_t> find_start_and_end_of_literal(const Constituent& l);
  void                                fixed_point_anchored_deduce();
  void                                fixed_point_unanchored_deduce();
  IndexPair                           match(const Constituent c);
  IndexPair                           match(const std::string_view literal);
  void                                deduce_recursion_on_left(Constituent& r, Constituent& c2);
  void                                deduce_recursion_on_right(Constituent& c1, Constituent& r);
  bool                                unanchored_deduce(Constituent& c1, Constituent& c2);

  // PRIVATE DATA
  bool                                 recursive = false;
  // TODO make static, so alternatives can share context
  std::unordered_map<int, std::string> character_map;
  std::unordered_map<int, std::string> string_map;
  std::vector<Constituent>             constituents;
  const std::string_view               string;
  std::size_t                          index = 0;
};

#endif
