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

#include "Parser.hpp"
#include "generated/NeothueLexer.h"
#include "generated/ThueLexer.h"
#include "terminal.h"
#include "util.h"
#include <cstdlib>
#include <iostream>
#include <string>

using neothue::NeothueParserImplementation;
using neothue::ThueParserImplementation;

int main(int argc, char* argv[])
{
  try {
    const Config config = parse_command_line_options(argc, argv);

    if (config.print_usage) {
      usage();
      return EXIT_SUCCESS;
    }

    std::string source_code = file_as_string(config.filename);

    if (config.classic) {
      preprocess(source_code);

      ThueLexer                                   lexer(source_code);
      Parser<ThueLexer, ThueParserImplementation> parser(lexer, config.filename.data(), source_code);

      auto [grammar, initial_state] = parser.parse(config.order, config.classic, config.debug);

      grammar.apply_productions(initial_state);
    }
    else {
      NeothueLexer                                      lexer(source_code);
      Parser<NeothueLexer, NeothueParserImplementation> parser(lexer, config.filename.data(), source_code);

      auto [grammar, initial_state] = parser.parse(config.order, config.classic, config.debug);

      grammar.apply_productions(initial_state);
    }
  }
  catch (const File_not_found& fnf) {
    std::cerr << bold(red("error: ")) << "File not found: " << bold(fnf.file) << '\n';
    return EXIT_FAILURE;
  }
  //  catch (const Lexer::Syntax_error& se) {
  //    std::cerr << underline() << se.number << (se.number > 1 ? " errors" : " error") << " in total" << reset() <<
  //    '\n'; return EXIT_FAILURE;
  //  }
  catch (const Parse_error& se) {
    std::cerr << underline() << se.number << (se.number > 1 ? " errors" : " error") << " in total" << reset() << '\n';
    return EXIT_FAILURE;
  }
  catch (...) {
    std::cerr << "Unknown exception occurred.\n";
    return EXIT_FAILURE;
  }
}
