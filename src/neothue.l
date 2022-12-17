%top {
    #include <iostream>
    #include <algorithm>
    #include "Token.h"
    #include "ParserImplementation.h"
%}

%{
    using nthue::position;

    static position yypos;
    static position yytemp;
    static std::string yybuf;
%}

%option bison-complete
%option bison-locations
%option bison-cc-namespace=nthue
%option bison-cc-parser=ParserImplementation
%option header-file="Lexer.h"
%option lexer=Lexer
%option lex=lex
%option namespace=nthue
%option outfile="Lexer.cpp"
%option noyywrap

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

    nthue::location pos = location();
    pos.begin = yytemp;

    return ParserImplementation::make_STRING(yybuf, pos);
}

<INITIAL>["] start(DQSTR); yytemp = location().begin;
<DQSTR>{dq_str} yybuf += str();
<DQSTR>[\\]["] yybuf += '"';
<DQSTR>[\\]. yybuf += text();
<DQSTR>["]  {
    yybuf.clear();
    start(INITIAL);

    nthue::location pos = location();
    pos.begin = yytemp;

    return ParserImplementation::make_STRING(yybuf, pos);
}

<UNQSTR>{unquoted_str}  {
    start(INITIAL);

    return ParserImplementation::make_STRING(str(), location());
}

<INITIAL>{comment}
<INITIAL>{ws}+
<INITIAL>[=] return ParserImplementation::make_SEPARATOR(location());
<INITIAL>. start(UNQSTR); matcher().unput(chr());
