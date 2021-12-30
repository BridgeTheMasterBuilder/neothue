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

#ifndef TOKEN_H
#define TOKEN_H

#include <string>

struct Token {
  // ENUMS
  enum Type
  {
    START,
    STRING,
    SEPARATOR,
    END,
    NONE
  };

  // CONSTRUCTORS
  Token(const Type type, const char c, const int line = 1, const int col = 1)
      : type(type), value(std::string(1, c)), line(line), col(col)
  { }
  Token(const Type type, const std::string value = "", const int line = 1, const int col = 1)
      : type(type), value(value), line(line), col(col)
  { }

  // PUBLIC MEMBER FUNCTIONS
  auto size() const noexcept
  {
    if (type == Token::Type::STRING) return value.size() + 2;
    else return value.size();
  }

  // CONVERSION OPERATORS
  operator const char*() const noexcept { return value.c_str(); }
  operator Type() const noexcept { return type; }

  // PUBLIC DATA
  const Type        type;
  const std::string value;
  const int         line;
  const int         col;
};

#endif
