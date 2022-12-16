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

#ifndef LEXER_H
#define LEXER_H

#include <deque>
#include <string>
#include <string_view>
#include "Parser.h"
#include "Token.h"

namespace nthue {
  using Token = Parser::symbol_type;

  class Lexer {
  public:
    // CONSTRUCTORS
    Lexer(const std::string& source_code, const std::string_view filename);

    // EXCEPTIONS
    struct Syntax_error {
      Syntax_error(const int number) : number(number) { }

      const int number;
    };

    struct Unterminated_string { };

    // PUBLIC MEMBER FUNCTIONS
    Token        lex();

  private:
    // PRIVATE MEMBER FUNCTIONS
    bool        finished_scanning() const noexcept { return index >= source_code.size(); }
    void        emit_error(const std::string_view message) const;
    std::size_t find_end_of_line() const noexcept;
    std::size_t find_end_of_string(const std::size_t start_of_string) const;
    std::size_t find_start_of_line() const noexcept;
    std::size_t find_start_of_string(const std::size_t index) const noexcept;
    int         get() noexcept;
    auto        length_of_string(const std::string_view string) const noexcept;
    char        peek() const noexcept { return !finished_scanning() ? source_code[index] : EOF; }
    void        report_error() const;
    void        skip_comment() noexcept;
    void        skip_whitespace() noexcept;
    void        synchronize(const std::size_t old_index) noexcept;

    // TOKENIZERS
    Token separator();
    Token  string();

    // TODO don't forget to remove column_number and line_number
    // PRIVATE DATA
    int                          column_number = 0;
    const std::string_view       filename;
    std::size_t                  index            = 0;
    int                          line_number      = 1;
    mutable bool                 malformed        = false;
    mutable int                  number_of_errors = 0;
    const std::string&           source_code;
    position                     pos;
    static constexpr const char* delimiters = "\t\n\r ;=}";
  };
}

#endif
