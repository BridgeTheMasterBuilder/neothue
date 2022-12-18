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

%class{
    const std::string_view neothue_source_filename; 
    const std::string& neothue_source_code;
}

%option ctorarg="const std::string_view neothue_source_filename, const std::string& neothue_source_code"
%option ctorinit="neothue_source_filename(neothue_source_filename), neothue_source_code(neothue_source_code)"

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

%top {
    #include <iostream>
    #include <algorithm>
%}

%{
    #include "../Lexer.h"

    using neothue::position;

    static position yytemp;
    static std::string yybuf;
    static bool in_string = false;
%}

ws [ \t\n\r\f\v]
unquoted_str [^=; \t\n\r\f\v]+
sq_str [^\\']*
dq_str [^\\"]*
comment [;][^\n]*

%x UNQSTR SQSTR DQSTR

%%

<INITIAL>['] {
    in_string = true;
    yytemp = location().begin;
    yybuf.clear();
    start(SQSTR);
}
<SQSTR>{sq_str} yybuf += str();
<SQSTR>[\\]['] yybuf += '\'';
<SQSTR>[\\]. yybuf += text();
<SQSTR>[']  {
    in_string = false;
    start(INITIAL);

    neothue::location pos = location();
    pos.begin = yytemp;

    return ThueParserImplementation::make_STRING(yybuf, pos);
}

<INITIAL>["] {
    in_string = true;
    yytemp = location().begin;
    yybuf.clear();
    start(DQSTR);
}
<DQSTR>{dq_str} yybuf += str();
<DQSTR>[\\]["] yybuf += '"';
<DQSTR>[\\]. yybuf += text();
<DQSTR>["]  {
    in_string = false;
    start(INITIAL);

    neothue::location pos = location();
    pos.begin = yytemp;

    return ThueParserImplementation::make_STRING(yybuf, pos);
}

<UNQSTR>{unquoted_str}  {
    start(INITIAL);

    return ThueParserImplementation::make_STRING(str(), location());
}

<INITIAL>{comment}
<INITIAL>{ws}+
<INITIAL>[=] return ThueParserImplementation::make_SEPARATOR(location());
<INITIAL>. start(UNQSTR); matcher().unput(chr());
<<EOF>> {
    if(in_string) {
        neothue::location loc = location();

        const std::size_t size = neothue_source_code.size()-1;

        if(neothue_source_code[size] == '\n') {
            std::size_t start_of_line = neothue_source_code.rfind('\n', size-1);

            const std::size_t length = size - (start_of_line == std::string::npos ? 1 : start_of_line);

            loc.begin.line -= 1;
            loc.begin.column += length;
            loc.end.line -= 1;
            loc.end.column += length;
        }

        report_syntax_error(neothue_source_filename, neothue_source_code, loc);
        throw Unterminated_string();
    }
    else {
        return ThueParserImplementation::make_YYEOF(location());
    }
}

