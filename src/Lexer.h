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

#ifndef LEXER_H
#define LEXER_H

#include "Token.hpp"
#include <deque>
#include <string>
#include <string_view>

class Lexer {
public:
  // CONSTRUCTORS
  Lexer(const std::string& source_code, const std::string_view filename);

  // EXCEPTIONS
  struct Syntax_error {
    Syntax_error(const int number) : number(number) { }

    const int number;
  };

  struct Unterminated_string {
  };

  // PUBLIC MEMBER FUNCTIONS
  bool         empty() const { return tokens.front() == Token::Type::END; }
  const Token& lookahead() const noexcept;
  const Token& lookaround() const noexcept;
  Token        pop() noexcept;
  void         push(const Token token) { tokens.push_back(token); }

private:
  // PRIVATE MEMBER FUNCTIONS
  bool        finished_scanning() const noexcept { return index >= source_code.size(); }
  void        emit_error(const std::string_view message) const;
  std::size_t find_end_of_line() const noexcept;
  std::size_t find_end_of_string(const std::size_t start_of_string) const;
  std::size_t find_start_of_line() const noexcept;
  std::size_t find_start_of_string(const std::size_t index) const noexcept;
  int         get() noexcept;
  Token       get_token();
  auto        length_of_string(const std::string_view string) const noexcept;
  char        peek() const noexcept { return !finished_scanning() ? source_code[index] : EOF; }
  void        report_error() const;
  void        skip_comment() noexcept;
  void        skip_whitespace() noexcept;
  void        synchronize(const std::size_t old_index) noexcept;

  // TOKENIZERS
  Token end_of_pattern();
  Token pattern_separator();
  Token separator();
  Token start_of_pattern();
  Token string();
  Token tokenize(auto type);

  // PRIVATE DATA
  int                          column_number = 0;
  const std::string_view       filename;
  std::size_t                  index            = 0;
  int                          line_number      = 1;
  mutable bool                 malformed        = false;
  mutable int                  number_of_errors = 0;
  const std::string&           source_code;
  std::deque<Token>            tokens;
  static constexpr const char* delimiters = "\t\n\r =}";
};

#endif
