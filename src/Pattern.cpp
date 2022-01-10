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

#include "Pattern.h"

/**************************
 * PUBLIC MEMBER FUNCTIONS *
 **************************/
void Pattern::add_alternative(const std::string_view pattern)
{
  if (representative.empty()) representative = pattern;

  _alternatives.push_back(analyze_pattern(pattern.data()));
}

// TODO add ability to escape 'c' and 's'
// TODO maybe also add a syntax for for example c1...c...c1 which would match c1c2c3...cN...c3c2c1 etc.
/***************************
 * PRIVATE MEMBER FUNCTIONS *
 ***************************/
Alternative Pattern::analyze_pattern(const std::string& pattern)
{
  Alternative constituents;

  constituents.push_back(Start());

  for (std::size_t i = 0; i < pattern.size();) {
    switch (pattern[i]) {
      case '.':
        {
          const std::string ellipsis = pattern.substr(i, 3);

          if (ellipsis == "...") {
            constituents.push_back(Recursion());
            i += 3;
          }
          else i++;
        }
        break;
      case 'c':
        {
          const auto [number, end] = parse_id(pattern, i + 1);
          constituents.push_back(Character(number));
          i = end;
        }
        continue;
      case 's':
        {
          const auto [number, end] = parse_id(pattern, i + 1);
          constituents.push_back(String(number));
          i = end;
        }
        continue;
      // case '\\':
      //   i++;
      //   [[fallthrough]];
      default:
        const auto [literal, end] = parse_literal(pattern, i);
        constituents.push_back(Literal(literal));
        i = end;
        continue;
    }
  }

  constituents.push_back(End());

  return constituents;
}

std::pair<int, std::size_t> Pattern::parse_id(const std::string& pattern, const std::size_t index)
{
  static int  number = 0;
  std::size_t end    = index;

  if (std::isdigit(pattern[index])) {
    end = pattern.find_first_not_of("0123456789", index);
    if (end == std::string::npos) end = pattern.size();

    number = std::strtol(pattern.substr(index, end - index).c_str(), nullptr, 10);
  }
  else number++;

  return { number, end };
}

std::pair<std::string, std::size_t> Pattern::parse_literal(const std::string& pattern, const std::size_t index)
{
  std::size_t end;

  while (true) {
    end = pattern.find_first_of("cs.", index);

    if (end == std::string::npos) {
      end = pattern.size();
      break;
    }
    else if (pattern[end] == '.') {
      const std::string ellipsis = pattern.substr(end, 3);

      if (ellipsis != "...") continue;
    }

    if (pattern[end - 1] == '\\') continue;
    else break;
  }

  const std::string literal = pattern.substr(index, end - index);

  return { literal, end };
}
