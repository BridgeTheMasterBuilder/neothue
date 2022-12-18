import sys

if len(sys.argv) > 3 or not (sys.argv[1].casefold() == 'neothue' or sys.argv[1].casefold() == 'thue'):
    print('Usage: python generate_lexer.py (thue|neothue) grammar_file')
    sys.exit(1)

language = sys.argv[1].capitalize()
grammar_file = open(sys.argv[2], 'r')

grammar = grammar_file.read()
grammar_file.close()

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

{grammar}"""

generated_lexer = open(f'generated/{language.lower()}.l', 'w')
generated_lexer.write(lexer)
