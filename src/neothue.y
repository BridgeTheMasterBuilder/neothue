%require "3.8.2"
%language "C++"

%define api.namespace {nthue}
%define api.parser.class {Parser}
%define api.value.type variant
%define api.token.constructor
%define api.location.file "Token.h"
%define parse.assert
%define parse.error detailed
%define parse.lac full

%locations
%header "Parser.h"
%output "Parser.cpp"

%token SEPARATOR "="
%token <std::string> STRING "string"

%code requires {
    namespace nthue {
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
namespace nthue {
    // TODO get total number of errors?
    // TODO look up error message
    void Parser::error(const location_type& loc, const std::string& msg) {
        std::cerr << loc << msg << '\n';
    }
}
