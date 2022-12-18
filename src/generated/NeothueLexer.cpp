// generated/NeothueLexer.cpp generated by reflex 3.2.11 from /home/master/projects/thue/src/generated/neothue.l

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
#define REFLEX_OPTION_YYSTYPE             neothue::NeothueParserImplementation::semantic_type
#define REFLEX_OPTION_bison_cc            true
#define REFLEX_OPTION_bison_cc_namespace  neothue
#define REFLEX_OPTION_bison_cc_parser     NeothueParserImplementation
#define REFLEX_OPTION_bison_complete      true
#define REFLEX_OPTION_bison_locations     true
#define REFLEX_OPTION_ctorarg             const std::string_view neothue_source_filename, const std::string& neothue_source_code
#define REFLEX_OPTION_ctorinit            neothue_source_filename(neothue_source_filename), neothue_source_code(neothue_source_code)
#define REFLEX_OPTION_header_file         "generated/NeothueLexer.h"
#define REFLEX_OPTION_lex                 lex
#define REFLEX_OPTION_lexer               NeothueLexer
#define REFLEX_OPTION_namespace           neothue
#define REFLEX_OPTION_noyywrap            true
#define REFLEX_OPTION_outfile             "generated/NeothueLexer.cpp"
#define REFLEX_OPTION_token_eof           neothue::NeothueParserImplementation::symbol_type(0, location())
#define REFLEX_OPTION_token_type          neothue::NeothueParserImplementation::symbol_type

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 1: %top user code                                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#line 19 "/home/master/projects/thue/src/generated/neothue.l"

    #include "Token.h"
    #include "NeothueParserImplementation.h"

#line 66 "/home/master/projects/thue/src/generated/neothue.l"

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

class NeothueLexer : public reflex::AbstractLexer<reflex::Matcher> {
#line 35 "/home/master/projects/thue/src/generated/neothue.l"

    const std::string_view neothue_source_filename;
    const std::string& neothue_source_code;

 public:
  typedef reflex::AbstractLexer<reflex::Matcher> AbstractBaseLexer;
  NeothueLexer(
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
  virtual neothue::NeothueParserImplementation::symbol_type lex(void);
};

} // namespace neothue

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 1: %{ user code %}                                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#line 1 "/home/master/projects/thue/src/generated/neothue.l"
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

#line 43 "/home/master/projects/thue/src/generated/neothue.l"

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

#line 71 "/home/master/projects/thue/src/generated/neothue.l"

    #include "../Lexer.h"

    using neothue::position;

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

neothue::NeothueParserImplementation::symbol_type neothue::NeothueLexer::lex(void)
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
#line 131 "/home/master/projects/thue/src/generated/neothue.l"
{
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
}
            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule /home/master/projects/thue/src/generated/neothue.l:91: ['] :
#line 91 "/home/master/projects/thue/src/generated/neothue.l"
in_string = true; start(SQSTR); yytemp = location().begin;
            break;
          case 2: // rule /home/master/projects/thue/src/generated/neothue.l:106: ["] :
#line 106 "/home/master/projects/thue/src/generated/neothue.l"
in_string = true; start(DQSTR); yytemp = location().begin;
            break;
          case 3: // rule /home/master/projects/thue/src/generated/neothue.l:127: {comment} :
#line 127 "/home/master/projects/thue/src/generated/neothue.l"
            break;
          case 4: // rule /home/master/projects/thue/src/generated/neothue.l:128: {ws}+ :
#line 128 "/home/master/projects/thue/src/generated/neothue.l"
            break;
          case 5: // rule /home/master/projects/thue/src/generated/neothue.l:129: [=] :
#line 129 "/home/master/projects/thue/src/generated/neothue.l"
return NeothueParserImplementation::make_SEPARATOR(location());
            break;
          case 6: // rule /home/master/projects/thue/src/generated/neothue.l:130: . :
#line 130 "/home/master/projects/thue/src/generated/neothue.l"
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
#line 131 "/home/master/projects/thue/src/generated/neothue.l"
{
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
}
            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule /home/master/projects/thue/src/generated/neothue.l:121: {unquoted_str} :
#line 121 "/home/master/projects/thue/src/generated/neothue.l"
{
    start(INITIAL);

    return NeothueParserImplementation::make_STRING(str(), location());
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
#line 131 "/home/master/projects/thue/src/generated/neothue.l"
{
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
}
            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule /home/master/projects/thue/src/generated/neothue.l:92: {sq_str} :
#line 92 "/home/master/projects/thue/src/generated/neothue.l"
yybuf += str();
            break;
          case 2: // rule /home/master/projects/thue/src/generated/neothue.l:93: [\\]['] :
#line 93 "/home/master/projects/thue/src/generated/neothue.l"
yybuf += '\'';
            break;
          case 3: // rule /home/master/projects/thue/src/generated/neothue.l:94: [\\]. :
#line 94 "/home/master/projects/thue/src/generated/neothue.l"
yybuf += text();
            break;
          case 4: // rule /home/master/projects/thue/src/generated/neothue.l:95: ['] :
#line 95 "/home/master/projects/thue/src/generated/neothue.l"
{
    in_string = false;
    yybuf.clear();
    start(INITIAL);

    neothue::location pos = location();
    pos.begin = yytemp;

    return NeothueParserImplementation::make_STRING(yybuf, pos);
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
#line 131 "/home/master/projects/thue/src/generated/neothue.l"
{
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
}
            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule /home/master/projects/thue/src/generated/neothue.l:107: {dq_str} :
#line 107 "/home/master/projects/thue/src/generated/neothue.l"
yybuf += str();
            break;
          case 2: // rule /home/master/projects/thue/src/generated/neothue.l:108: [\\]["] :
#line 108 "/home/master/projects/thue/src/generated/neothue.l"
yybuf += '"';
            break;
          case 3: // rule /home/master/projects/thue/src/generated/neothue.l:109: [\\]. :
#line 109 "/home/master/projects/thue/src/generated/neothue.l"
yybuf += text();
            break;
          case 4: // rule /home/master/projects/thue/src/generated/neothue.l:110: ["] :
#line 110 "/home/master/projects/thue/src/generated/neothue.l"
{
    in_string = false;
    yybuf.clear();
    start(INITIAL);

    neothue::location pos = location();
    pos.begin = yytemp;

    return NeothueParserImplementation::make_STRING(yybuf, pos);
}

            break;
        }
        break;
      default:
        start(0);
    }
  }
}
