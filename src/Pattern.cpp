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
#include <unordered_map>

std::unordered_map<int, std::string> Pattern::map;

/**************************
 * PUBLIC MEMBER FUNCTIONS *
 **************************/
void Pattern::add_alternative(const std::string_view pattern)
{
  if (representative.empty()) representative = pattern;

  _alternatives.push_back(analyze_pattern(pattern.data()));
}

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
        continue;
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
      case '\\':
        i++;
        [[fallthrough]];
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
  static int                           number = 0;
  static std::unordered_map<char, int> index_map;
  std::size_t                          end = index;

  if (pattern[index] != '_') return { number++, end++ };

  if (std::isdigit(pattern[index + 1])) {
    end = pattern.find_first_not_of("0123456789", index + 1);
    if (end == std::string::npos) end = pattern.size();

    number = std::strtol(pattern.substr(index + 1, end - index).c_str(), nullptr, 10);

    return { number, end };
  }
  else {
    if (!index_map.contains(pattern[index + 1])) index_map[pattern[index + 1]] = -(++number);

    end = index + 2;

    return { index_map[pattern[index + 1]], end };
  }
}

std::pair<std::string, std::size_t> Pattern::parse_literal(const std::string& pattern, const std::size_t index)
{
  std::size_t end;

  while (true) {
    end = pattern.find_first_of("cs.", index + 1);

    if (end == std::string::npos) {
      end = pattern.size();
      break;
    }

    if (pattern[end - 1] == '\\') continue;
    else break;
  }

  const std::string literal = pattern.substr(index, end - index);

  return { literal, end };
}
