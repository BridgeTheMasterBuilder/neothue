// generated/ThueLexer.cpp generated by reflex 3.2.11 from /home/master/projects/thue/src/generated/thue.l

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

#line 66 "/home/master/projects/thue/src/generated/thue.l"

    #include <iostream>
    #include <algorithm>


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
#line 35 "/home/master/projects/thue/src/generated/thue.l"

    const std::string_view neothue_source_filename;
    const std::string& neothue_source_code;

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
  static const int UNQSTR = 1;
  static const int SQSTR = 2;
  static const int DQSTR = 3;
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
//  SECTION 1: %{ user code %}                                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#line 1 "/home/master/projects/thue/src/generated/thue.l"
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

#line 43 "/home/master/projects/thue/src/generated/thue.l"

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

#line 71 "/home/master/projects/thue/src/generated/thue.l"

    #include "../Lexer.h"

    using neothue::position;

    static position yypos;
    static position yytemp;
    static std::string yybuf;
    static bool in_string = false;


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  BISON C++                                                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 2: rules                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

neothue::ThueParserImplementation::symbol_type neothue::ThueLexer::lex(void)
{
  static const char *REGEX_INITIAL = "(?m)(['])|([\"])|((?:[;][^\\x0a]*))|((?:[\\x09-\\x0d\\x20])+)|([=])|(.)";
  static const reflex::Pattern PATTERN_INITIAL(REGEX_INITIAL);
  static const char *REGEX_UNQSTR = "(?m)((?:[^\\x09-\\x0d\\x20;=]+))";
  static const reflex::Pattern PATTERN_UNQSTR(REGEX_UNQSTR);
  static const char *REGEX_SQSTR = "(?m)((?:[^'\\x5c]*))|([\\x5c]['])|([\\x5c].)|(['])";
  static const reflex::Pattern PATTERN_SQSTR(REGEX_SQSTR);
  static const char *REGEX_DQSTR = "(?m)((?:[^\"\\x5c]*))|([\\x5c][\"])|([\\x5c].)|([\"])";
  static const reflex::Pattern PATTERN_DQSTR(REGEX_DQSTR);
  if (!has_matcher())
  {
    matcher(new Matcher(PATTERN_INITIAL, stdinit(), this));
  }
  while (true)
  {
    switch (start())
    {
      case INITIAL:
        matcher().pattern(PATTERN_INITIAL);
        switch (matcher().scan())
        {
          case 0:
            if (matcher().at_end())
            {
#line 132 "/home/master/projects/thue/src/generated/thue.l"
{
    if(in_string) {
        report_syntax_error(neothue_source_filename, neothue_source_code, location());
        throw Unterminated_string();
    }
}
            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule /home/master/projects/thue/src/generated/thue.l:92: ['] :
#line 92 "/home/master/projects/thue/src/generated/thue.l"
in_string = true; start(SQSTR); yytemp = location().begin;
            break;
          case 2: // rule /home/master/projects/thue/src/generated/thue.l:107: ["] :
#line 107 "/home/master/projects/thue/src/generated/thue.l"
in_string = true; start(DQSTR); yytemp = location().begin;
            break;
          case 3: // rule /home/master/projects/thue/src/generated/thue.l:128: {comment} :
#line 128 "/home/master/projects/thue/src/generated/thue.l"
            break;
          case 4: // rule /home/master/projects/thue/src/generated/thue.l:129: {ws}+ :
#line 129 "/home/master/projects/thue/src/generated/thue.l"
            break;
          case 5: // rule /home/master/projects/thue/src/generated/thue.l:130: [=] :
#line 130 "/home/master/projects/thue/src/generated/thue.l"
return ThueParserImplementation::make_SEPARATOR(location());
            break;
          case 6: // rule /home/master/projects/thue/src/generated/thue.l:131: . :
#line 131 "/home/master/projects/thue/src/generated/thue.l"
start(UNQSTR); matcher().unput(chr());
            break;
        }
        break;
      case UNQSTR:
        matcher().pattern(PATTERN_UNQSTR);
        switch (matcher().scan())
        {
          case 0:
            if (matcher().at_end())
            {
#line 132 "/home/master/projects/thue/src/generated/thue.l"
{
    if(in_string) {
        report_syntax_error(neothue_source_filename, neothue_source_code, location());
        throw Unterminated_string();
    }
}
            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule /home/master/projects/thue/src/generated/thue.l:122: {unquoted_str} :
#line 122 "/home/master/projects/thue/src/generated/thue.l"
{
    start(INITIAL);

    return ThueParserImplementation::make_STRING(str(), location());
}

            break;
        }
        break;
      case SQSTR:
        matcher().pattern(PATTERN_SQSTR);
        switch (matcher().scan())
        {
          case 0:
            if (matcher().at_end())
            {
#line 132 "/home/master/projects/thue/src/generated/thue.l"
{
    if(in_string) {
        report_syntax_error(neothue_source_filename, neothue_source_code, location());
        throw Unterminated_string();
    }
}
            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule /home/master/projects/thue/src/generated/thue.l:93: {sq_str} :
#line 93 "/home/master/projects/thue/src/generated/thue.l"
yybuf += str();
            break;
          case 2: // rule /home/master/projects/thue/src/generated/thue.l:94: [\\]['] :
#line 94 "/home/master/projects/thue/src/generated/thue.l"
yybuf += '\'';
            break;
          case 3: // rule /home/master/projects/thue/src/generated/thue.l:95: [\\]. :
#line 95 "/home/master/projects/thue/src/generated/thue.l"
yybuf += text();
            break;
          case 4: // rule /home/master/projects/thue/src/generated/thue.l:96: ['] :
#line 96 "/home/master/projects/thue/src/generated/thue.l"
{
    in_string = false;
    yybuf.clear();
    start(INITIAL);

    neothue::location pos = location();
    pos.begin = yytemp;

    return ThueParserImplementation::make_STRING(yybuf, pos);
}

            break;
        }
        break;
      case DQSTR:
        matcher().pattern(PATTERN_DQSTR);
        switch (matcher().scan())
        {
          case 0:
            if (matcher().at_end())
            {
#line 132 "/home/master/projects/thue/src/generated/thue.l"
{
    if(in_string) {
        report_syntax_error(neothue_source_filename, neothue_source_code, location());
        throw Unterminated_string();
    }
}
            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule /home/master/projects/thue/src/generated/thue.l:108: {dq_str} :
#line 108 "/home/master/projects/thue/src/generated/thue.l"
yybuf += str();
            break;
          case 2: // rule /home/master/projects/thue/src/generated/thue.l:109: [\\]["] :
#line 109 "/home/master/projects/thue/src/generated/thue.l"
yybuf += '"';
            break;
          case 3: // rule /home/master/projects/thue/src/generated/thue.l:110: [\\]. :
#line 110 "/home/master/projects/thue/src/generated/thue.l"
yybuf += text();
            break;
          case 4: // rule /home/master/projects/thue/src/generated/thue.l:111: ["] :
#line 111 "/home/master/projects/thue/src/generated/thue.l"
{
    in_string = false;
    yybuf.clear();
    start(INITIAL);

    neothue::location pos = location();
    pos.begin = yytemp;

    return ThueParserImplementation::make_STRING(yybuf, pos);
}

            break;
        }
        break;
      default:
        start(0);
    }
  }
}
