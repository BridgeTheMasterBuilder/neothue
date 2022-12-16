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
%type <std::pair<Grammar, std::string>> program
%type <std::vector<Production>> productions
%type <std::string> initial_state
%type <Production> production

%code requires {
    #include "Grammar.h"

    namespace nthue {
        class Lexer;
    }
 }

%parse-param {Lexer& lexer}
%parse-param {Grammar& grammar}
%parse-param {std::string& initial_state}
%parse-param {application_order order}
%parse-param {bool classic}
%parse-param {bool debug}

%code {
#include "Lexer.h"
#define yylex lexer.lex

static std::vector<Production> productions;
 }

%%

program: productions initial_state { grammar = Grammar(productions, order, classic, debug); grammar.sort(); initial_state = $2; }

productions: productions production { productions.push_back($2); }
| ;

production: STRING SEPARATOR STRING { $$ = std::make_pair($1, $3); }

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
