#include "Lexer.h"
#include "Parser.h"
#include <boost/ut.hpp>
#include <fstream>
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

void test(const std::string item, const std::string message, const bool expected_result)
{
  bool successful = true;

  try {
    Lexer  l(item, "test");
    Parser p(l, "test", item);
    p.parse();
  }
  catch (...) {
    successful = false;
  }

  boost::ut::expect(successful == expected_result) << message;
}

int main()
{
  // TODO have to figure this out, can't hardcode the path like this
  std::ifstream i("/home/master/projects/thue/tests/testdata");
  if (!i) std::cerr << "FUCK";
  std::vector<std::string> strings;

  while (i) {
    std::string temp;
    std::getline(i, temp);
    strings.push_back(temp);
  }

  for (const auto& program : strings) {
    test(program, "\nParser did not recognize:\n\t" + program + "\nas a valid program", true);
  }

  const std::vector<std::string> illegal_programs = {
    "'a'b", "\"a\"b", "'a''b'", "'a'\"b\"", "\"a\"'b'", "\"a\"\"b\""
  };

  for (const auto& program : illegal_programs) {
    test(program, "\nParser incorrectly recognized:\n\t" + program + "\nas a valid program", false);
  }
}
