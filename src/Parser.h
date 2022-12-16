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

#include "Lexer.h"
#include "Grammar.h"
#include <string>
#include "ParserImplementation.h"

namespace nthue {
  class Parser {
  public:
    // CONSTRUCTORS
    Parser(Lexer& lexer);

    // PUBLIC MEMBER FUNCTIONS
    std::pair<Grammar, std::string> parse(const application_order order, const bool classic, const bool debug);

    // EXCEPTIONS
    struct Syntax_error {
      Syntax_error(const int number) : number(number) { }

      const int number;
    };

  private:
    // PRIVATE DATA
    int number_of_errors = 0;
    Lexer lexer;
  };
}

#endif
