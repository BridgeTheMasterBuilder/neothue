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

#include "Lexer.h"
#include "terminal.h"
#include <iostream>
#include <algorithm>

namespace nthue {
  /***************
 * CONSTRUCTORS *
 ***************/
  Lexer::Lexer(const std::string& source_code, const std::string_view filename)
      : filename(filename.empty() ? "stdin" : filename), source_code(source_code), pos(position())
  {
    if (malformed) throw Syntax_error(number_of_errors);
  }

  /**************************
 * PUBLIC MEMBER FUNCTIONS *
 **************************/
    Token Lexer::lex()
    {
      try {
        const int lookahead = peek();

        switch (lookahead) {
          case '\n':
          case '\t':
          case '\r':
          case ' ':
            skip_whitespace();
            return lex();
          case ';':
            skip_comment();
            return lex();
          case '=':
            return separator();
          case EOF:
            return ParserImplementation::make_YYEOF(location(pos));
          default:
            return string();
        }
      }
      catch (const Unterminated_string& us) {
        report_error();
        throw Syntax_error(number_of_errors);
      }
    }

  /***************************
 * PRIVATE MEMBER FUNCTIONS *
 ***************************/
  void Lexer::emit_error(const std::string_view message) const
  {
    const std::size_t start = find_start_of_line();
    const std::size_t end   = find_end_of_line();

    std::string line        = source_code.substr(start, end - start);

    line.append(underline(bold(red(" "))));

    const auto [_, line_num, col_num] = pos;

    std::cerr << bold() << filename << ':' << line_num << ':' << col_num << ": " << bold(red("error: "))
              << reset();

    std::cerr << message << '\n';

    std::cerr << '\t' << line_num << " | " << line << '\n';
  }

  std::size_t Lexer::find_end_of_line() const noexcept
  {
    std::size_t end_of_line = source_code.find('\n', index);

    if (end_of_line == std::string::npos) end_of_line = source_code.size();

    return end_of_line;
  }

  std::size_t Lexer::find_end_of_string(const std::size_t start_of_string) const
  {
    const char quote = source_code[index];

    std::size_t end_of_string;

    if (quote == '\'' || quote == '\"') {
      end_of_string = source_code.find(quote, start_of_string);

      // TODO expand escapes
      while (end_of_string != std::string::npos && source_code[end_of_string - 1] == '\\')
        end_of_string = source_code.find(quote, end_of_string + 1);

      if (end_of_string == std::string::npos) {
        end_of_string = source_code.size();
        throw Unterminated_string();
      }
    }
    else {
      end_of_string = source_code.find_first_of(delimiters, start_of_string);
      if (end_of_string == std::string::npos) end_of_string = source_code.size();
    }

    return end_of_string;
  }

  std::size_t Lexer::find_start_of_line() const noexcept
  {
    std::size_t start_of_line = source_code.rfind('\n', index);

    if (start_of_line == std::string::npos) start_of_line = 0;
    else start_of_line++;

    return start_of_line;
  }

  std::size_t Lexer::find_start_of_string(const std::size_t index) const noexcept
  {
    const char quote = source_code[index];

    if (quote == '\'' || quote == '\"' || quote == '\\') return index + 1;
    else return index;
  }

  int Lexer::get() noexcept
  {
    pos += 1;
    return !finished_scanning() ? source_code[index++] : EOF;
  }


  auto Lexer::length_of_string(const std::string_view string) const noexcept
  {
    const char quote = source_code[index];

    if (quote == '\'' || quote == '\"') return string.size() + 2;
    else return string.size();
  }

  void Lexer::report_error() const
  {
    malformed = true;
    number_of_errors++;

    emit_error("Unterminated string");
  }

  void Lexer::skip_comment() noexcept { index = find_end_of_line() + 1; }

  void Lexer::skip_whitespace() noexcept
  {
    while (!finished_scanning()) {
      switch (peek()) {
        case '\n':
          pos.lines();
          index++;
          break;
        case '\t':
        case '\r':
        case ' ':
          get();
          continue;
        default:
          goto finished;
      }
    }

  finished:;
  }

  void Lexer::synchronize(const std::size_t old_index) noexcept
  {
    const char quote = source_code[old_index];

    if (quote == '\'' || quote == '\"') index++;
  }

  /*************
 * TOKENIZERS *
 *************/
  Token Lexer::separator() {
    get();

    return ParserImplementation::make_SEPARATOR(location()); }

  Token  Lexer::string()
  {
    const std::size_t start_of_string = find_start_of_string(index);
    const std::size_t end_of_string   = find_end_of_string(start_of_string);
    const std::string string          = source_code.substr(start_of_string, end_of_string - start_of_string);
    const std::size_t length          = length_of_string(string);
    const std::size_t old_index       = index;
    const std::size_t number_of_newlines = std::count(string.begin(), string.end(), '\n');

    index                             = end_of_string;
    auto start = pos;
    pos += length;
    pos.lines(number_of_newlines);
    auto end = pos;

    synchronize(old_index);

    return ParserImplementation::make_STRING(string, location(start, end));
  }
}