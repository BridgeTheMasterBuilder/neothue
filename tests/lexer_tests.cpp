#include "Lexer.h"
#include <boost/ut.hpp>
#include <strings.h>

/*
  program = { production }, [ initial state ] ;
  production = string, "=", string ;
  initial state = { symbol } ;
  string = "'", { symbol - "'" }, "'"
         | "\"", { symbol - "\"" }, "\"" ;
  symbol = ? any character ? ;

  microsyntax using unary strings:

  string = '\'', { 1 }, '\'' ;
         | '\"', { 1 }, '\"' ;
         | { 1 } ;

  separator = '=' ;

  RE: ('1*'|"1*"|1*)|=
 */

void test(const std::string item,
          const Token::Type type,
          const std::string message,
          const bool        expected_result,
          const bool        compare_external = true)
{
  bool types_match = true;

  std::string representation;
  bool        representations_match = true;

  try {
    const Token t = Lexer(item, "test").pop();
    if (t != type) types_match = false;
    if (compare_external && strcasecmp(t, item.c_str()) != 0) {
      representation        = static_cast<std::string>(t);
      representations_match = false;
    }
  }
  catch (...) {
    types_match = false;
    if (compare_external) representations_match = false;
  }

  boost::ut::expect(types_match == expected_result) << message;
  boost::ut::expect(representations_match) << "Lexer incorrectly parsed " << item << " as " << representation;
}

int main()
{
  const std::vector<std::string> legal_strings = { "\"\"", "\"1\"", "\"11\"", "''",    "'1'",   "'11'",    "1", "11",
                                                   "111",  "1111",  "'~'",    "':::'", "\"~\"", "\":::\"", "~", ":::" };

  for (const auto& item : legal_strings) {
    test(item, Token::Type::STRING, "Lexer did not recognize " + item + " as a string.", true, false);
  }

  const std::vector<std::string> illegal_strings = { "\"", "\"'", "\"1", "\"1'", "\"11", "\"11'",
                                                     "'",  "'\"", "'1",  "'1\"", "'11",  "'11\"" };

  for (const auto& item : illegal_strings) {
    test(item, Token::Type::STRING, "Lexer incorrectly recognized " + item + " as a string.", false, false);
  }

  /***************
   * MISC. TOKENS *
   ***************/
  test("=", Token::Type::SEPARATOR, "Lexer did not recognize = as a separator.", true);
  test("; foo bar baz", Token::Type::STRING, "Lexer did not recognize ; foo bar baz as a comment.", false, false);
}
