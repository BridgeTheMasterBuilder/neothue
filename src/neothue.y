%require "3.8.2"
%language "C++"

%define api.value.type variant
%define api.token.constructor
%define parse.assert
%define parse.error detailed
%define parse.lac full

%locations
%header

%token SEPARATOR "="
%token <std::string> STRING "string"

%code {
    namespace yy {
        parser::symbol_type yylex() {
            static bool foo = false;

            if (!foo) {
                foo = true;
                return parser::make_SEPARATOR(parser::location_type());
            } else {
                return parser::make_YYEOF(parser::location_type());
            }
        }
    }
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
    void parser::error(const location_type& loc, const std::string& msg) {
        std::cerr << loc << msg << '\n';
    }
}
