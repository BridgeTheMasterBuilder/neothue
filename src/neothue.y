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
%type <std::string> initial_state

%code requires {
    #include "Grammar.h"

    namespace nthue {
        class Lexer;
    }
 }

%parse-param {Lexer& lexer}
%parse-param {Grammar& grammar}
%parse-param {std::string& initial_state}

%code {
#include "Lexer.h"
#define yylex lexer.lex
 }

%%

program: productions initial_state { grammar.sort(); initial_state = $2; }

productions: productions production
| ;

production: STRING SEPARATOR STRING { grammar.add_production($1, $3); }

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
