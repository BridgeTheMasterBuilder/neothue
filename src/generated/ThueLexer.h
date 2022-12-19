// generated/ThueLexer.h generated by reflex 3.2.11 from /home/master/projects/thue/src/generated/thue.l

#ifndef REFLEX_GENERATED_THUELEXER_H
#define REFLEX_GENERATED_THUELEXER_H
#define IN_HEADER 1
#define REFLEX_VERSION "3.2.11"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  OPTIONS USED                                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#undef REFLEX_OPTION_YYLTYPE
#undef REFLEX_OPTION_YYSTYPE
#undef REFLEX_OPTION_bison_cc
#undef REFLEX_OPTION_bison_cc_namespace
#undef REFLEX_OPTION_bison_cc_parser
#undef REFLEX_OPTION_bison_complete
#undef REFLEX_OPTION_bison_locations
#undef REFLEX_OPTION_ctorarg
#undef REFLEX_OPTION_ctorinit
#undef REFLEX_OPTION_header_file
#undef REFLEX_OPTION_lex
#undef REFLEX_OPTION_lexer
#undef REFLEX_OPTION_namespace
#undef REFLEX_OPTION_nodefault
#undef REFLEX_OPTION_noyywrap
#undef REFLEX_OPTION_outfile
#undef REFLEX_OPTION_token_eof
#undef REFLEX_OPTION_token_type

#define REFLEX_OPTION_YYLTYPE             neothue::location
#define REFLEX_OPTION_YYSTYPE             neothue::ThueParserImplementation::semantic_type
#define REFLEX_OPTION_bison_cc            true
#define REFLEX_OPTION_bison_cc_namespace  neothue
#define REFLEX_OPTION_bison_cc_parser     ThueParserImplementation
#define REFLEX_OPTION_bison_complete      true
#define REFLEX_OPTION_bison_locations     true
#define REFLEX_OPTION_ctorarg             const std::string_view neothue_source_filename, const std::string& neothue_source_code
#define REFLEX_OPTION_ctorinit            neothue_source_filename(neothue_source_filename), neothue_source_code(neothue_source_code)
#define REFLEX_OPTION_header_file         "generated/ThueLexer.h"
#define REFLEX_OPTION_lex                 lex
#define REFLEX_OPTION_lexer               ThueLexer
#define REFLEX_OPTION_namespace           neothue
#define REFLEX_OPTION_nodefault           true
#define REFLEX_OPTION_noyywrap            true
#define REFLEX_OPTION_outfile             "generated/ThueLexer.cpp"
#define REFLEX_OPTION_token_eof           neothue::ThueParserImplementation::symbol_type(0, location())
#define REFLEX_OPTION_token_type          neothue::ThueParserImplementation::symbol_type

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 1: %top user code                                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#line 19 "/home/master/projects/thue/src/generated/thue.l"

    #include "Token.h"
    #include "ThueParserImplementation.h"


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  REGEX MATCHER                                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/matcher.h>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  ABSTRACT LEXER CLASS                                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/abslexer.h>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  LEXER CLASS                                                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

namespace neothue {

class ThueLexer : public reflex::AbstractLexer<reflex::Matcher> {
#line 36 "/home/master/projects/thue/src/generated/thue.l"

    [[maybe_unused]] const std::string_view neothue_source_filename;
    [[maybe_unused]] const std::string& neothue_source_code;

 public:
  typedef reflex::AbstractLexer<reflex::Matcher> AbstractBaseLexer;
  ThueLexer(
      const std::string_view neothue_source_filename, const std::string& neothue_source_code,
      // a persistent source of input, empty by default
      const reflex::Input& input = reflex::Input(),
      // optional output stream, std::cout by default
      std::ostream& os = std::cout)
    :
      AbstractBaseLexer(input, os),
      neothue_source_filename(neothue_source_filename), neothue_source_code(neothue_source_code)
  {
  }
  static const int INITIAL = 0;
  static const int LHS = 1;
  static const int SEP = 2;
  static const int RHS = 3;
  static const int STATE = 4;
  std::string filename;
  // bison-complete bison-locations: location() returns lexeme location
  virtual neothue::location location(void)
  {
    neothue::location yylloc;
    yylloc.begin.filename = &filename;
    yylloc.begin.line = static_cast<unsigned int>(matcher().lineno());
    yylloc.begin.column = static_cast<unsigned int>(matcher().columno()+1);
    yylloc.end.filename = &filename;
    yylloc.end.line = static_cast<unsigned int>(matcher().lineno_end());
    yylloc.end.column = static_cast<unsigned int>(matcher().columno_end()+2);
    return yylloc;
  }
  // the bison-complete lexer function defined by SECTION 2
  virtual neothue::ThueParserImplementation::symbol_type lex(void);
};

} // namespace neothue

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  BISON C++                                                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#endif
