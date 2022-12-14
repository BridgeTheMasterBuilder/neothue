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

#include "terminal.h"
#include "util.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include "Lexer.h"

using yy::Lexer;

int main(int argc, char* argv[])
{
  try {
    const Config config = parse_command_line_options(argc, argv);

    if (config.print_usage) {
      usage();
      return EXIT_SUCCESS;
    }

    std::string source_code = file_as_string(config.filename);

    if (config.classic) preprocess(source_code);

    Lexer  lexer(source_code, config.filename.data());
//
//    auto [grammar, initial_state] = parser.parse(config.order, config.classic, config.debug);
//
//    grammar.apply_productions(initial_state);

    yy::parser parser(lexer);
    // TODO get total number of errors?
    if (parser.parse() != 0) throw 0;
  }
  catch (const File_not_found& fnf) {
    std::cerr << bold(red("error: ")) << "File not found: " << bold(fnf.file) << '\n';
    return EXIT_FAILURE;
  }
  catch (const Lexer::Syntax_error& se) {
    std::cerr << underline() << se.number << (se.number > 1 ? " errors" : " error") << " in total" << reset() << '\n';
    return EXIT_FAILURE;
  }
//  catch (const Parser::Syntax_error& se) {
//    std::cerr << underline() << se.number << (se.number > 1 ? " errors" : " error") << " in total" << reset() << '\n';
//    return EXIT_FAILURE;
//  }
  catch (...) {
    std::cerr << "Unknown exception occurred.\n";
    return EXIT_FAILURE;
  }
}
