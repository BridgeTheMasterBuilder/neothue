#include "Pattern.h"

void Pattern::add_alternative(const std::string_view pattern)
{
  if (representative.empty()) representative = pattern;

  _alternatives.push_back(analyze_pattern(pattern.data()));
}

// TODO ability to escape 'c' and 's'
Alternative Pattern::analyze_pattern(const std::string& pattern)
{
  Alternative constituents;

  for (std::size_t i = 0; i < pattern.size();) {
    switch (pattern[i]) {
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

// TODO verify this later
std::pair<std::string, std::size_t> Pattern::parse_literal(const std::string& pattern, const std::size_t index)
{
  std::size_t end;
  while (true) {
    end = pattern.find_first_of("cs", index);

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
