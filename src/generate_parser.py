import sys

if len(sys.argv) > 3 or not (sys.argv[1].casefold() == 'neothue' or sys.argv[1].casefold() == 'thue'):
    print('Usage: python generate_parser.py (thue|neothue) grammar_file')
    sys.exit(1)

language = sys.argv[1].capitalize()
grammar_file = open(sys.argv[2], 'r')

grammar = grammar_file.read()
grammar_file.close()

parser = fr"""/*
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

%require "3.8.2"
%language "C++"

%define api.namespace {{neothue}}
%define api.parser.class {{{language}ParserImplementation}}
%define api.value.type variant
%define api.token.constructor
%define api.location.file "Token.h"
%define parse.error custom

%locations
%header "generated/{language}ParserImplementation.h"
%output "generated/{language}ParserImplementation.cpp"

%code requires {{
    #include "../Grammar.h"

    namespace neothue {{
        class {language}Lexer;
    }}
 }}

%parse-param {{{language}Lexer& lexer}}
%parse-param {{Grammar& grammar}}
%parse-param {{std::string& initial_state}}
%parse-param {{int& number_of_errors}}
%parse-param {{const std::string_view filename}}
%parse-param {{const std::string& source_code}}

%code {{
#include <string>
#include "../util.h"
#include "{language}Lexer.h"
#define yylex lexer.lex
 }}

{grammar}

%%
namespace neothue {{
    void {language}ParserImplementation::error(const location_type& loc, const std::string& msg) {{
        std::cerr << loc << msg << '\n';
    }}

    void {language}ParserImplementation::report_syntax_error(const {language}ParserImplementation::context& ctx) const {{
      number_of_errors++;

      const auto& location = ctx.location();
      const auto& start_pos = location.begin;
      const auto& end_pos = location.end;
      const auto [_, line_num, col_num] = start_pos;
      int length = (end_pos.column - start_pos.column) + (end_pos.line - start_pos.line);

      report_error(filename, source_code, line_num, col_num, length);
    }}
}}"""

generated_parser = open(f'generated/{language.lower()}.y', 'w')
generated_parser.write(parser)
