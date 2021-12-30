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
#include <iostream>

/***************
 * CONSTRUCTORS *
 ***************/
Grammar::Grammar(const application_order order, const bool debug) : order(order), debug(debug)
{
  std::random_device rd;
  rng.seed(rd());
}

/**************************
 * PUBLIC MEMBER FUNCTIONS *
 **************************/
void Grammar::add_production(const std::string_view lhs, const std::string_view rhs)
{
  productions.emplace_back(lhs, rhs);
}

void Grammar::apply_productions(std::string& initial_state)
{
  if (debug) std::cout << "Initial state: \"" << initial_state << "\"\n";

  while (true) {
    bool match_found = false;

    for (auto& production : productions) {
      const auto& lhs                  = production.first;

      const std::size_t index_of_match = match(lhs, initial_state);

      if (index_of_match == std::string::npos) continue;
      else match_found = true;

      apply_production(production, initial_state, index_of_match);

      if (debug) std::cout << initial_state << '\n';
    }

    const size_t index_of_input = match(":::", initial_state);

    if (index_of_input != std::string::npos) {
      match_found = true;

      std::string input;
      std::getline(std::cin, input);
      if (!std::cin.eof()) input += '\n';

      initial_state.replace(index_of_input, 3, input);
    }

    const size_t index_of_output = match('~', initial_state);

    if (index_of_output != std::string::npos) {
      match_found = true;

      std::cout << initial_state.substr(index_of_output + 1);

      initial_state.erase(index_of_output, 1);
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
void Grammar::apply_production(Production& production, std::string& string, const std::size_t index_of_match)
{
  const auto& [lhs, rhs] = production;

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

void Grammar::rewrite_production(Production& production, const std::string_view rhs) { production.second = rhs; }

void Grammar::shuffle() { std::shuffle(productions.begin(), productions.end(), rng); }

void Grammar::sort_left_to_right() { std::sort(productions.begin(), productions.end()); }

void Grammar::sort_right_to_left() { std::sort(productions.rbegin(), productions.rend()); }
