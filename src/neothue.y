%require "3.8.2"
%language "C++"

%define api.namespace {nthue}
%define api.parser.class {ParserImplementation}
%define api.value.type variant
%define api.token.constructor
%define api.location.file "Token.h"
%define parse.assert
%define parse.error detailed
%define parse.lac full

%locations
%header "ParserImplementation.h"
%output "ParserImplementation.cpp"
%expect 2

%token SEPARATOR "="
%token <std::string> STRING "string"
%type <std::string> initial_state
%type <std::string> string

%code requires {
    #include "Grammar.h"

    namespace nthue {
        class Lexer;
    }
 }

%parse-param {Lexer& lexer}
%parse-param {Grammar& grammar}
%parse-param {std::string& initial_state}
%parse-param {int& number_of_errors}

%code {
#include "Lexer.h"
#define yylex lexer.lex
 }

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
namespace nthue {
    // TODO look up error message
    void ParserImplementation::error(const location_type& loc, const std::string& msg) {
        number_of_errors++;
        std::cerr << loc << msg << '\n';
    }
}
