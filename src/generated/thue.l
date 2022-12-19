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

%top {
    #include "Token.h"
    #include "ThueParserImplementation.h"
%}

%option bison-complete
%option bison-locations
%option bison-cc-namespace=neothue
%option bison-cc-parser=ThueParserImplementation
%option header-file="generated/ThueLexer.h"
%option lexer=ThueLexer
%option lex=lex
%option namespace=neothue
%option outfile="generated/ThueLexer.cpp"
%option noyywrap
%option nodefault

%class{
    [[maybe_unused]] const std::string_view neothue_source_filename; 
    [[maybe_unused]] const std::string& neothue_source_code;
}

%option ctorarg="const std::string_view neothue_source_filename, const std::string& neothue_source_code"
%option ctorinit="neothue_source_filename(neothue_source_filename), neothue_source_code(neothue_source_code)" 



%{
    #include "../Lexer.h"

    using neothue::position;

    static position marker;
%}

ws [ \t\n\r\f\v]
lhs [^:\n]*
rhs [^\n]*
sep [:][:][=]
empty_production {ws}*{sep}{ws}*

%x LHS SEP RHS STATE

%%

<INITIAL>. start(LHS); matcher().unput(chr());
<LHS>[\n]
<LHS>{empty_production} start(STATE);
<LHS>{lhs}/{sep} start(SEP); return ThueParserImplementation::make_STRING(str(), location());
<SEP>{sep} start(RHS); return ThueParserImplementation::make_SEPARATOR(location());
<RHS>{rhs} start(LHS); return ThueParserImplementation::make_STRING(str(), location());
<RHS>[\n] start(LHS); return ThueParserImplementation::make_STRING("", location());
<STATE>[^\n](.|\n)*/[\n]? start(LHS); return ThueParserImplementation::make_STRING(str(), location());
