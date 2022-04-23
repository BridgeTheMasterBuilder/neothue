#include "../src/Grammar.h"
#include "../src/Lexer.h"
#include "../src/Parser.h"
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

void test(const std::string item,
          const std::string message,
          const std::string expected_result,
          const bool        expect_success = true)
{
  bool        successful = true;
  std::string actual;

  try {
    Lexer  l(item, "test");
    Parser p(l, "test", item);
    auto [grammar, initial_state] = p.parse();
    grammar.apply_productions(initial_state);
    actual = initial_state;
    if (actual != expected_result && expect_success) successful = false;
  }
  catch (...) {
    successful = false;
  }

  boost::ut::expect(successful) << message << "got" << actual;
}

int main()
{
  const std::vector<std::pair<std::string, std::string>> patterns = {
    {"{c}=b a",                                      "b"   },
    { "{s}=b foo",                                   "b"   },
    { "{foo}=b foo",                                 "b"   },
    { "{cc}=b ab",                                   "b"   },
    { "{cs}=b afoo",                                 "b"   },
    { "{cfoo}=b afoo",                               "b"   },
    { "{sc}=b fooa",                                 "b"   },
    { "{sfoo}=b barfoo",                             "b"   },
    { "{fooc}=b fooa",                               "b"   },
    { "{foos}=b foobar",                             "b"   },
    { "{ccc}=b abc",                                 "b"   },
    { "{ccs}=b abfoo",                               "b"   },
    { "{ccfoo}=b abfoo",                             "b"   },
    { "{csc}=b afoob",                               "b"   },
    { "{cfooc}=b afoob",                             "b"   },
    { "{cfoos}=b afoobar",                           "b"   },
    { "{cfoobar}=b afoobar",                         "b"   },
    { "{scfoo}=b barafoo",                           "b"   },
    { "{sfooc}=b barfoob",                           "b"   },
    { "{sfoos}=b barfoobaz",                         "b"   },
    { "{sfoobaz}=b barfoobaz",                       "b"   },
    { "{foocc}=b fooab",                             "b"   },
    { "{foocs}=b fooabar",                           "b"   },
    { "{foocbar}=b fooabar",                         "b"   },
    { "{foosbaz}=b foobarbaz",                       "b"   },
    { "{foobarc}=b foobara",                         "b"   },
    { "{foobars}=b foobarbaz",                       "b"   },
    { "{foobarbaz}=b foobarbaz",                     "b"   },
 //
    { "{c}=b ab",                                    "bb"  },
    { "{foo}=b barfoo",                              "barb"},
    { "{cfoo}=b abfoo",                              "ab"  },
    { "{sfoo}=b barfoobaz",                          "bbaz"},
    { "{fooc}=b fooab",                              "bb"  },
    { "{foos}=b afoobar",                            "ab"  },
    { "{ccfoo}=b abfooc",                            "bc"  },
    { "{cfooc}=b abfoocd",                           "abd" },
    { "{cfoos}=b abfoobar",                          "ab"  },
    { "{cfoobar}=b abfoobar",                        "ab"  },
    { "{scfoo}=b barafooa",                          "ba"  },
    { "{sfooc}=b barfooba",                          "ba"  },
    { "{sfoobaz}=b barfoobaza",                      "ba"  },
    { "{foocc}=b afooabc",                           "abc" },
    { "{foocs}=b afooabar",                          "ab"  },
    { "{foocbar}=b afooabarb",                       "abb" },
    { "{foosbaz}=b afoobarbazb",                     "abb" },
    { "{foobarc}=b afoobarab",                       "abb" },
    { "{foobars}=b afoobarbaz",                      "ab"  },
    { "{foobarbaz}=b afoobarbazb",                   "abb" },
    { "{c_1c_1}=b aa",                               "b"   },
    { "{s_1as_1}=b fooafoo",                         "b"   },
    { "{c_1...c_1}=b abba",                          "b"   },
    { "{c_1...c_1}=b abccba",                        "b"   },
    { "{c_1...c_1 | c_2}=b aba",                     "b"   },
    { "{c_1...c_1 | c_2}=b abcba",                   "b"   },
    { "{c_1...c_1 | foo...foo}=b afoofooa",          "b"   },
    { "{c_1...c_1 | foo...foo}=b afoobbfooa",        "b"   },
    { "{c_1...c_1 | foo...foo | bar}=b foobarfoo",   "b"   },
    { "{c_1...c_1 | foo...foo | bar}=b fooabarafoo", "b"   },
  };

  for (const auto& [grammar, result] : patterns)
    test(grammar, "Failed to match " + grammar, result);

  const std::vector<std::pair<std::string, std::string>> ambiguous_patterns = {
    {"{css}=b afoobar",                                    "b"},
    { "{csfoo}=b abarfoo",                                 "b"},
    { "{scc}=b fooab",                                     "b"},
    { "{scs}=b fooabar",                                   "b"},
    { "{ssc}=b barfooa",                                   "b"},
    { "{sss}=b barfoobaz",                                 "b"},
    { "{ssbaz}=b barfoobaz",                               "b"},
    { "{foosc}=b foobara",                                 "b"},
    { "{fooss}=b foobarbaz",                               "b"},
    { "{c_1c_1}=b ab",                                     "b"},
    { "{s_1as_1}=b fooabar",                               "b"},
    { "{c_1...c_1}=b abca",                                "b"},
    { "{c_1...c_1}=b abccca",                              "b"},
    { "{c_1...c_1}=b abcdba",                              "b"},
    { "{c_1...c_1 | foo...foo}=b afoobara",                "b"},
    { "{c_1...c_1 | foo...foo}=b bfoofooa",                "b"},
    { "{c_1...c_1 | foo...foo}=b bfoobara",                "b"},
    { "{c_1...c_1 | foo...foo}=b afoobcfooa",              "b"},
    { "{c_1...c_1 | foo...foo | bar}=b foobarbaz",         "b"},
    { "{c_1...c_1 | foo...foo | bar}=b fooabarbfoo",       "b"},
    { "{c_1...c_1 | foo...foo | bar}=b fooafoobarfoobfoo", "b"},
    { "{c_1...c_1 | foo...foo | bar}=b fooafoobarfooafoo", "b"},
    { "{cc}=b abc",                                        "b"},
    { "{ccc}=b abcd",                                      "b"},
  };

  for (const auto& [grammar, result] : ambiguous_patterns)
    test(grammar, "Incorrectly matched " + grammar, result, false);
}
