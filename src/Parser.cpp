/*
    thue, an interpreter for the Thue metalanguage
    Copyright (C) 2021  masterbuilder

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

#include "Parser.h"
#include "terminal.h"
#include <iostream>

/**************************
 * PUBLIC MEMBER FUNCTIONS *
 **************************/
std::pair<Grammar, std::string> Parser::parse(const application_order order, const bool classic, const bool debug)
{
  Grammar grammar(order, classic, debug);

  while (!finished_parsing()) {
    try {
      if (lexer.lookahead() == Token::Type::START_OF_PATTERN) {
        const Pattern lhs = pattern();
        separator();
        const std::string rhs = string();

        grammar.add_production(lhs, rhs);
      }
      else {
        const std::string lhs = string();
        separator();
        if (lexer.lookahead() == Token::Type::START_OF_PATTERN) {
          const Pattern rhs = pattern();

          // define pattern
        }
        const std::string rhs = string();

        grammar.add_production(lhs, rhs);
      }
    }
    catch (const Bad_sentence& bs) {
      report_error(bs.token);
      continue;
    }
  }

  if (malformed) throw Syntax_error(number_of_errors);

  grammar.sort();

  return { grammar, lexer.empty() ? "" : string() };
}

/***************************
 * PRIVATE MEMBER FUNCTIONS *
 ***************************/
Token Parser::expect(const Token::Type type)
{
  const Token& t = lexer.lookahead();

  if (t.type != type) throw Bad_sentence(type, t);

  return lexer.pop();
}

std::size_t Parser::find_end_of_line(const std::size_t start) const noexcept
{
  std::size_t end_of_line = source_code.find('\n', start);

  if (end_of_line == std::string::npos) end_of_line = source_code.size();

  return end_of_line;
}

std::size_t Parser::find_start_of_line(const int line_number) const noexcept
{
  std::size_t start_of_line = 0;

  for (int i = 1; i < line_number; i++)
    start_of_line = source_code.find('\n', start_of_line) + 1;

  return start_of_line;
}

bool Parser::finished_parsing() const noexcept
{
  return lexer.empty()
         || ((lexer.lookahead() == Token::Type::STRING || lexer.lookahead() == Token::Type::END_OF_PATTERN)
             && lexer.lookaround() == Token::Type::END);
}

// TODO better error handling
void Parser::report_error(const Token token)
{
  malformed = true;
  number_of_errors++;

  const std::size_t start  = find_start_of_line(token.line);
  const std::size_t end    = find_end_of_line(start);
  const std::size_t length = token.size();

  std::string line         = source_code.substr(start, end - start);

  std::cerr << bold() << filename << ':' << token.line << ':' << token.col << ": " << bold(red("error: ")) << reset();

  std::cerr
      << "Expected to find \x1B[1m=\x1B[0m but instead found a string. Note that productions must be separated by "
         "\x1B[1m=\x1B[0m\n";

  line.replace(token.col - 1, length, underline(bold(red(line.substr(token.col - 1, length)))));

  std::cerr << '\t' << token.line << " | " << line << '\n';

  synchronize();
}

// TODO is the synchronization correct?
void Parser::synchronize() noexcept
{
  Token::Type previous = Token::Type::STRING;

  while (!lexer.empty()
         && !((previous == Token::Type::STRING || previous == Token::Type::END_OF_PATTERN)
              && (lexer.lookahead() == Token::Type::STRING || lexer.lookahead() == Token::Type::START_OF_PATTERN)))
    previous = lexer.pop().type;
}

/**********
 * PARSERS *
 **********/
Pattern Parser::pattern()
{
  Pattern pattern;

  lexer.pop();

  while (!lexer.empty()) {
    pattern.add_alternative(string());
    if (lexer.lookahead() == Token::Type::END_OF_PATTERN) break;
    expect(Token::Type::PATTERN_SEPARATOR);
  }

  lexer.pop();

  for (const auto& alternative : pattern.alternatives()) {
    for (const auto& constituent : alternative) {
      std::visit([](const auto arg) { std::cout << arg.value << '\n'; }, constituent);
    }
  }

  return pattern;
}

std::string Parser::string()
{
  const Token& t = lexer.lookahead();

  if (t.type != Token::Type::STRING) return "";
  else return lexer.pop().value;
}
