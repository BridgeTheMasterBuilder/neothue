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

%require "3.8.2"
%language "C++"

%define api.namespace {neothue}
%define api.parser.class {NeothueParserImplementation}
%define api.value.type variant
%define api.token.constructor
%define api.location.file "Token.h"
%define parse.error custom

%locations
%header "generated/NeothueParserImplementation.h"
%output "generated/NeothueParserImplementation.cpp"

%code requires {
    #include "../Grammar.h"

    namespace neothue {
        class NeothueLexer;
    }
 }

%parse-param {NeothueLexer& lexer}
%parse-param {Grammar& grammar}
%parse-param {std::string& initial_state}
%parse-param {int& number_of_errors}
%parse-param {const std::string_view filename}
%parse-param {const std::string& source_code}

%code {
#include <string>
#include "../util.h"
#include "NeothueLexer.h"
#define yylex lexer.lex
 }

%expect 2

%token SEPARATOR "="
%token <std::string> STRING "string"
%type <std::string> initial_state
%type <std::string> string

%%

program: productions initial_state { grammar.sort(); initial_state = $2; }

productions: productions production
| %empty

production: string SEPARATOR string { grammar.add_production($1, $3); }
| string error string

initial_state: string

string: STRING
| %empty { $$ = ""; } %expect 2

%%
namespace neothue {
    void NeothueParserImplementation::error(const location_type& loc, const std::string& msg) {
        std::cerr << loc << msg << '\n';
    }

    void NeothueParserImplementation::report_syntax_error(const NeothueParserImplementation::context& ctx) const {
      number_of_errors++;

      const auto& location = ctx.location();
      const auto& start_pos = location.begin;
      const auto& end_pos = location.end;
      const auto [_, line_num, col_num] = start_pos;
      int length = (end_pos.column - start_pos.column) + (end_pos.line - start_pos.line);

      report_error(filename, source_code, line_num, col_num, length);
    }
}