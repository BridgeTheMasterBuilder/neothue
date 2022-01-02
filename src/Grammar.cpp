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

#include "Grammar.h"
#include "util.h"
#include <cstring>
#include <iostream>

/***************
 * CONSTRUCTORS *
 ***************/
Grammar::Grammar(const application_order order, const bool classic, const bool debug)
    : order(order), classic(classic), debug(debug)
{
  if (order == application_order::NONDETERMINISTIC) {
    std::random_device rd;
    rng.seed(rd());
  }
}

/**************************
 * PUBLIC MEMBER FUNCTIONS *
 **************************/
void Grammar::apply_productions(std::string& initial_state)
{
  if (debug) std::cout << "Initial state: \"" << initial_state << "\"\n";

  while (true) {
    bool match_found = false;

    for (auto& production : productions) {
      const auto& lhs = production.first;

      const auto [start, end] =
          std::visit([this, &initial_state](
                         const auto& lhs) -> std::pair<std::size_t, std::size_t> { return match(lhs, initial_state); },
                     lhs);

      if (start == std::string::npos) continue;
      else match_found = true;

      apply_production(production, initial_state, start, end);

      if (debug) std::cout << initial_state << '\n';
    }

    // TODO abstract away into a function?
    if (!classic) {
      const auto [index_of_input, end_of_input] = match(":::", initial_state);

      if (index_of_input != std::string::npos) {
        match_found = true;

        std::string input;
        std::getline(std::cin, input);
        if (!std::cin.eof()) input += '\n';

        initial_state.replace(index_of_input, 3, input);
      }

      const auto [index_of_output, end_of_output] = match('~', initial_state);

      if (index_of_output != std::string::npos) {
        match_found = true;

        std::cout << initial_state.substr(index_of_output + 1);

        initial_state.erase(index_of_output, 1);
      }
    }

    if (!match_found) break;
  }

  if (debug) std::cout << "Final state: \"" << initial_state << "\"\n";
}

void Grammar::sort()
{
  using enum application_order;

  if (order == LEFT_TO_RIGHT) sort_left_to_right();
  else if (order == RIGHT_TO_LEFT) sort_right_to_left();
  else shuffle();
}

/***************************
 * PRIVATE MEMBER FUNCTIONS *
 ***************************/
void Grammar::add_production(const std::variant<Pattern, std::string> lhs, const std::string_view rhs)
{
  productions.emplace_back(lhs, rhs);
}

void Grammar::apply_production(Production&       production,
                               std::string&      string,
                               const std::size_t start,
                               const std::size_t end)
{
  const auto& [lhs, rhs] = production;

  std::visit([this, &production, &string, start, end, &rhs](
                 const auto& lhs) { return apply_production(production, lhs, rhs, string, start, end); },
             lhs);
}

// TODO implement this
void Grammar::apply_production([[maybe_unused]] Production&        production,
                               [[maybe_unused]] const Pattern&     lhs,
                               [[maybe_unused]] const std::string& rhs,
                               [[maybe_unused]] std::string&       string,
                               [[maybe_unused]] const std::size_t  start,
                               [[maybe_unused]] const std::size_t  end)
{ }

void Grammar::apply_production(Production&                        production,
                               const std::string&                 lhs,
                               const std::string&                 rhs,
                               std::string&                       string,
                               const std::size_t                  index_of_match,
                               [[maybe_unused]] const std::size_t _)
{
  std::size_t index;

  if ((index = rhs.find(":::")) != std::string::npos) {
    std::string input;
    std::getline(std::cin, input);
    if (!std::cin.eof()) input += '\n';

    std::string new_rhs = rhs;
    new_rhs.replace(index, 3, input);

    rewrite_production(production, new_rhs);

    string.replace(index_of_match, lhs.size(), rhs);
  }
  else if ((index = rhs.find("~")) != std::string::npos) {
    std::cout << rhs.substr(index + 1);

    string.replace(index_of_match, lhs.size(), "");
  }
}

std::pair<std::size_t, std::size_t> Grammar::match(const char lhs, const std::string_view string)
{
  std::size_t index_of_match = locate(lhs, string);

  return { index_of_match, index_of_match + 1 };
}

std::pair<std::size_t, std::size_t> Grammar::match(const char* lhs, const std::string_view string)
{
  std::size_t index_of_match = locate(lhs, string);

  return { index_of_match, index_of_match + std::strlen(lhs) };
}

std::pair<std::size_t, std::size_t> Grammar::match(const std::string_view lhs, const std::string_view string)
{
  std::size_t index_of_match = locate(lhs, string);

  return { index_of_match, index_of_match + lhs.size() };
}

std::pair<std::size_t, std::size_t> Grammar::match(const std::string& lhs, const std::string_view string)
{
  return match(std::string_view(lhs), string);
}

// TODO implement this
std::pair<std::size_t, std::size_t> Grammar::match([[maybe_unused]] const Pattern&         lhs,
                                                   [[maybe_unused]] const std::string_view string)
{
  return { std::string::npos, std::string::npos };
}

void Grammar::rewrite_production(Production& production, const std::string_view rhs) { production.second = rhs; }

void Grammar::shuffle() { std::shuffle(productions.begin(), productions.end(), rng); }

void Grammar::sort_left_to_right() { std::sort(productions.begin(), productions.end()); }

void Grammar::sort_right_to_left() { std::sort(productions.rbegin(), productions.rend()); }
