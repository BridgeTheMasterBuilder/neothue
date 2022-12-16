#include "Lexer.h"
#include <boost/ut.hpp>
#include <strings.h>
#include "ParserImplementation.h"
#include "Token.h"

using nthue::Lexer;
using nthue::ParserImplementation;
using nthue::location;

void test(const std::string item,
          const nthue::Token type,
          const std::string message,
          const bool        expected_result,
          const bool        compare_external = true)
{
  bool types_match = true;

  std::string representation;
  bool        representations_match = true;

  try {
    const nthue::Token t = Lexer(item, "test").lex();
    if (t.kind() != type.kind()) types_match = false;
    if (compare_external && strcasecmp(t.name(), item.c_str()) != 0) {
      representation        = t.name();
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
    test(item, ParserImplementation::make_STRING("", location()), "Lexer did not recognize " + item + " as a string.", true, false);
  }

  const std::vector<std::string> illegal_strings = { "\"", "\"'", "\"1", "\"1'", "\"11", "\"11'",
                                                     "'",  "'\"", "'1",  "'1\"", "'11",  "'11\"" };

  for (const auto& item : illegal_strings) {
    test(item, ParserImplementation::make_STRING("", location()), "Lexer incorrectly recognized " + item + " as a string.", false, false);
  }

  /***************
   * MISC. TOKENS *
   ***************/
  test("=", ParserImplementation::make_SEPARATOR(location()), "Lexer did not recognize = as a separator.", true);
  test("; foo bar baz", ParserImplementation::make_STRING("", location()), "Lexer did not recognize ; foo bar baz as a comment.", false, false);
}
