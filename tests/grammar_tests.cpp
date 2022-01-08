#include "Lexer.h"
#include "Parser.h"
#include <boost/ut.hpp>
#include <string>
#include <vector>

/*
  program = { production }, [ initial state ] ;
  production = string, "=", string ;
  initial state = { symbol } ;
  string = "'", { symbol - "'" }, "'"
         | "\"", { symbol - "\"" }, "\"" ;
  symbol = ? any character ? ;

  syntax using unary strings:
  (( |\n|\t)*('1*'|"1*"|1*)( |\n|\t)*=( |\n|\t)*('1*'|"1*"|1*)( |\n|\t)*)*( |\n|\t)*('1*'|"1*"|1*)( |\n|\t)*)*
 */

void test(const std::string item, const std::string message, const std::string expected_result)
{
  bool        successful = true;
  std::string actual;

  try {
    Lexer  l(item, "test");
    Parser p(l, "test", item);
    auto [grammar, initial_state] = p.parse();
    grammar.apply_productions(initial_state);
    actual = initial_state;
    if (actual != expected_result) successful = false;
  }
  catch (...) {
    successful = false;
  }

  boost::ut::expect(successful) << message;
}

int main()
{
  const std::vector<std::pair<std::string, std::string>> legal_programs = {
    {"{c}=b a",                  "b"},
    { "{s}=b foo",               "b"},
    { "{foo}=b foo",             "b"},
    { "{cc}=b ab",               "b"},
    { "{cs}=b afoo",             "b"},
    { "{cfoo}=b afoo",           "b"},
    { "{sc}=b fooa",             "b"},
    { "{sfoo}=b barfoo",         "b"},
    { "{fooc}=b fooa",           "b"},
    { "{foos}=b foobar",         "b"},
    { "{ccc}=b abc",             "b"},
    { "{ccs}=b abfoo",           "b"},
    { "{ccfoo}=b abfoo",         "b"},
    { "{csc}=b afoob",           "b"},
    { "{css}=b afoobar",         "b"},
    { "{csfoo}=b abarfoo",       "b"},
    { "{cfooc}=b afoob",         "b"},
    { "{cfoos}=b afoobar",       "b"},
    { "{cfoobar}=b afoobar",     "b"},
    { "{scc}=b fooab",           "b"},
    { "{scs}=b fooabar",         "b"},
    { "{scfoo}=b barafoo",       "b"},
    { "{ssc}=b barfooa",         "b"},
    { "{sss}=b barfoobaz",       "b"},
    { "{ssbaz}=b barfoobaz",     "b"},
    { "{sfooc}=b barfoob",       "b"},
    { "{sfoos}=b barfoobaz",     "b"},
    { "{sfoobaz}=b barfoobaz",   "b"},
    { "{foocc}=b fooab",         "b"},
    { "{foocs}=b fooabar",       "b"},
    { "{foocbar}=b fooabar",     "b"},
    { "{foosc}=b foobara",       "b"},
    { "{fooss}=b foobarbaz",     "b"},
    { "{foosbaz}=b foobarbaz",   "b"},
    { "{foobarc}=b foobara",     "b"},
    { "{foobars}=b foobarbaz",   "b"},
    { "{foobarbaz}=b foobarbaz", "b"},
  };

  for (const auto& [grammar, result] : legal_programs) {
    test(grammar, "Grammar did not rewrite" + grammar + " to " + result, result);
  }
}