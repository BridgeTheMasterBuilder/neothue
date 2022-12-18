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
    #include "NeothueParserImplementation.h"
%}

%option bison-complete
%option bison-locations
%option bison-cc-namespace=neothue
%option bison-cc-parser=NeothueParserImplementation
%option header-file="generated/NeothueLexer.h"
%option lexer=NeothueLexer
%option lex=lex
%option namespace=neothue
%option outfile="generated/NeothueLexer.cpp"
%option noyywrap

%top {
    #include <iostream>
    #include <algorithm>
%}

%{
    using neothue::position;

    static position yypos;
    static position yytemp;
    static std::string yybuf;
%}

ws [ \t\n\r\f\v]
unquoted_str [^=; \t\n\r\f\v]+
sq_str [^\\']*
dq_str [^\\"]*
comment [;][^\n]*

%x UNQSTR SQSTR DQSTR

%%

<INITIAL>['] start(SQSTR); yytemp = location().begin;
<SQSTR>{sq_str} yybuf += str();
<SQSTR>[\\]['] yybuf += '\'';
<SQSTR>[\\]. yybuf += text();
<SQSTR>[']  {
    yybuf.clear();
    start(INITIAL);

    neothue::location pos = location();
    pos.begin = yytemp;

    return NeothueParserImplementation::make_STRING(yybuf, pos);
}

<INITIAL>["] start(DQSTR); yytemp = location().begin;
<DQSTR>{dq_str} yybuf += str();
<DQSTR>[\\]["] yybuf += '"';
<DQSTR>[\\]. yybuf += text();
<DQSTR>["]  {
    yybuf.clear();
    start(INITIAL);

    neothue::location pos = location();
    pos.begin = yytemp;

    return NeothueParserImplementation::make_STRING(yybuf, pos);
}

<UNQSTR>{unquoted_str}  {
    start(INITIAL);

    return NeothueParserImplementation::make_STRING(str(), location());
}

<INITIAL>{comment}
<INITIAL>{ws}+
<INITIAL>[=] return NeothueParserImplementation::make_SEPARATOR(location());
<INITIAL>. start(UNQSTR); matcher().unput(chr());
