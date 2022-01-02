#ifndef PATTERN_H
#define PATTERN_H

#include <string>
#include <string_view>
#include <variant>
#include <vector>

class Pattern {
public:
  // INTERNAL CLASSES
  class Character {
  public:
    Character(const int value) : value(value) { }

    operator int() { return value; }

    // private:
    const int value = 0;
  };

  struct String {
  public:
    String(const int value) : value(value) { }

    operator int() { return value; }

    // private:
    const int value = 0;
  };

  class Literal {
  public:
    Literal(const std::string_view value) : value(value) { }

    operator std::string_view() { return value; }

    // private:
    const std::string value;
  };

  // PUBLIC MEMBER FUNCTIONS
  void add_alternative(const std::string_view pattern)
  {
    if (representative.empty()) representative = pattern;

    _alternatives.push_back(analyze_pattern(pattern.data()));
  }

  const auto& alternatives() const { return _alternatives; }

  // FRIEND FUNCTIONS
  friend bool operator<(const Pattern& a, const Pattern& b) { return a.representative < b.representative; }

private:
  // PRIVATE MEMBER FUNCTIONS
  std::vector<std::variant<Character, String, Literal>> analyze_pattern(const std::string& pattern)
  {
    std::vector<std::variant<Character, String, Literal>> constituents;

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
        default:
          const auto [literal, end] = parse_literal(pattern, i);
          constituents.push_back(Literal(literal));
          i = end;
          continue;
      }
    }

    return constituents;
  }

  std::pair<int, std::size_t> parse_id(const std::string& pattern, const std::size_t index)
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

  std::pair<std::string, std::size_t> parse_literal(const std::string& pattern, const std::size_t index)
  {
    std::size_t end = pattern.find_first_of("cs", index);
    if (end == std::string::npos) end = pattern.size();

    const std::string literal = pattern.substr(index, end - index);

    return { literal, end };
  }

  // PRIVATE DATA
  std::string_view                                                   representative;
  std::vector<std::vector<std::variant<Character, String, Literal>>> _alternatives;
};

#endif
