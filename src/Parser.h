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

#ifndef PARSER_H
#define PARSER_H

#include "Token.hpp"
#include "Grammar.h"
#include "Lexer.h"
#include "Pattern.h"
#include <variant>

class Parser {
public:
  // CONSTRUCTORS
  Parser(Lexer& lexer, const std::string_view filename, const std::string& source_code)
      : filename(filename.empty() ? "stdin" : filename), lexer(lexer), source_code(source_code)
  { }

  // PUBLIC MEMBER FUNCTIONS
  std::pair<Grammar, std::string> parse(const application_order order   = application_order::NONDETERMINISTIC,
                                        const bool              classic = false,
                                        const bool              debug   = false);

  // EXCEPTIONS
  struct Bad_sentence {
    Bad_sentence(const Token::Type type, const Token token) : type(type), token(token) { }

    const Token::Type type;
    const Token       token;
  };

  struct Syntax_error {
    Syntax_error(const int number) : number(number) { }

    const int number;
  };

private:
  // PRIVATE MEMBER FUNCTIONS
  Token       expect(const Token::Type type);
  std::size_t find_end_of_line(const std::size_t start) const noexcept;
  std::size_t find_start_of_line(const int line_number) const noexcept;
  bool        finished_parsing() const noexcept;
  void        report_error(const Token token);
  void        synchronize() noexcept;

  // PARSERS
  Pattern                            pattern();
  std::variant<Pattern, std::string> pattern_or_string();
  void                               separator() { expect(Token::Type::SEPARATOR); }
  std::string                        string();

  // PRIVATE DATA
  const std::string_view       filename;
  Lexer&                       lexer;
  mutable bool                 malformed        = false;
  mutable int                  number_of_errors = 0;
  const std::string&           source_code;
  static constexpr const char* token_map[] = { "<", "string", "=", ">" };
};

#endif
