import sys

if len(sys.argv) > 3 or not (sys.argv[1].casefold() == 'neothue' or sys.argv[1].casefold() == 'thue'):
    print('Usage: python generate_lexer.py (thue|neothue) grammar_file')
    sys.exit(1)

language = sys.argv[1].capitalize()
grammar_file = open(sys.argv[2], 'r')

grammar = grammar_file.read()
grammar_file.close()

util = ''

if language == 'Neothue':
    util = r"""
%{
    #include "../util.h"
    #include "../terminal.h"
    
    static void report_syntax_error(const std::string_view neothue_source_filename, const std::string& neothue_source_code, const neothue::location& location) {
        const auto [_, line_num, col_num] = location.begin;

        const std::size_t start = find_start_of_line(neothue_source_code, line_num);
        const std::size_t end   = find_end_of_line(neothue_source_code, start);

        std::string line        = neothue_source_code.substr(start, end - start);

        line.append(underline(bold(red(" "))));

        std::cerr << bold() << neothue_source_filename << ':' << line_num << ':' << col_num << ": " << bold(red("error: "))
                  << reset();

        std::cerr << "Unterminated_string\n";

        std::cerr << '\t' << line_num << " | " << line << '\n';    
    }
%}
"""

lexer = fr"""/*
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

%top {{
    #include "Token.h"
    #include "{language}ParserImplementation.h"
%}}

%option bison-complete
%option bison-locations
%option bison-cc-namespace=neothue
%option bison-cc-parser={language}ParserImplementation
%option header-file="generated/{language}Lexer.h"
%option lexer={language}Lexer
%option lex=lex
%option namespace=neothue
%option outfile="generated/{language}Lexer.cpp"
%option noyywrap
%option nodefault

%class{{
    [[maybe_unused]] const std::string_view neothue_source_filename; 
    [[maybe_unused]] const std::string& neothue_source_code;
}}

%option ctorarg="const std::string_view neothue_source_filename, const std::string& neothue_source_code"
%option ctorinit="neothue_source_filename(neothue_source_filename), neothue_source_code(neothue_source_code)" 

{util}

{grammar}
"""

generated_lexer = open(f'generated/{language.lower()}.l', 'w')
generated_lexer.write(lexer)
