%require "3.8.2"
%language "C++"

%define api.namespace {yy}
%define api.parser.class {parser}
%define api.value.type variant
%define api.token.constructor
%define parse.assert
%define parse.error detailed
%define parse.lac full

%defines
%locations
%header

%token SEPARATOR "="
%token <std::string> STRING "string"

%code requires {
    namespace yy {
        class Lexer;
    }
 }

%parse-param {Lexer& lexer}

%code {
#include "Lexer.h"
#define yylex lexer.lex
 }

%%

program: productions initial_state ;

productions: productions production
| ;

production: STRING SEPARATOR STRING ;

initial_state: STRING
| ;

%%
namespace yy {
    // TODO get total number of errors?
    void parser::error(const location_type& loc, const std::string& msg) {
        std::cerr << loc << msg << '\n';
    }
}
