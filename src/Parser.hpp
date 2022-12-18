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

#ifndef PARSER_H
#define PARSER_H

#include "Grammar.h"
#include "generated/NeothueLexer.h"
#include "generated/NeothueParserImplementation.h"
#include "generated/ThueLexer.h"
#include "generated/ThueParserImplementation.h"
#include "terminal.h"
#include <iostream>
#include <string>
#include <concepts>

using neothue::NeothueLexer;
using neothue::ThueLexer;
using neothue::NeothueParserImplementation;
using neothue::ThueParserImplementation;

struct Parse_error {
  Parse_error(const int number) : number(number) { }

  const int number;
};

template<class Lexer, class ParserImplementation>
requires (std::same_as<Lexer, ThueLexer> && std::same_as<ParserImplementation, ThueParserImplementation> || std::same_as<Lexer, NeothueLexer> && std::same_as<ParserImplementation, NeothueParserImplementation>)
class Parser {
public:
  // CONSTRUCTORS
  Parser(Lexer& lexer, const std::string_view filename, const std::string& source_code)
      : filename(filename.empty() ? "stdin" : filename), lexer(lexer), source_code(source_code)
  { }

  // PUBLIC MEMBER FUNCTIONS
  std::pair<Grammar, std::string> parse(const application_order order = application_order::NONDETERMINISTIC, const bool classic = false, const bool debug = false)
  {
    Grammar     grammar(order, classic, debug);
    std::string initial_state;

    ParserImplementation parser(lexer, grammar, initial_state, number_of_errors, filename, source_code);

    if (parser.parse() != 0 || number_of_errors > 0) throw Parse_error(number_of_errors);

    return { grammar, initial_state };
  }

private:
  // PRIVATE DATA
  const std::string_view filename;
  Lexer&          lexer;
  const std::string&     source_code;
  int                    number_of_errors = 0;
};

#endif
