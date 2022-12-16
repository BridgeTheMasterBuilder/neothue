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

#include "Parser.h"
#include "terminal.h"
#include <iostream>

namespace nthue {
  /**************************
   * PUBLIC MEMBER FUNCTIONS *
   **************************/
  std::pair<Grammar, std::string> Parser::parse(const application_order order, const bool classic, const bool debug)
  {
    Grammar     grammar(order, classic, debug);
    std::string initial_state;

    ParserImplementation parser(lexer, grammar, initial_state, number_of_errors, filename, source_code);

    if (parser.parse() != 0 || number_of_errors > 0) throw Syntax_error(number_of_errors);

    return { grammar, initial_state };
  }
} // namespace nthue
