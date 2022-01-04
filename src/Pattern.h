#ifndef PATTERN_H
#define PATTERN_H

#include <string>
#include <string_view>
#include <variant>
#include <vector>

struct Character {
  Character(const int value) : value(value) { }

  operator int() { return value; }

  const int value = 0;
};

struct String {
  String(const int value) : value(value) { }

  operator int() { return value; }

  const int value = 0;
};

struct Literal {
  Literal(const std::string_view value) : value(value) { }

  operator std::string_view() { return value; }

  const std::string value;
};

using Alternative = std::vector<std::variant<Character, String, Literal>>;

class Pattern {
public:
  // PUBLIC MEMBER FUNCTIONS
  void        add_alternative(const std::string_view pattern);
  const auto& alternatives() const { return _alternatives; }

  // FRIEND FUNCTIONS
  friend bool operator<(const Pattern& a, const Pattern& b) { return a.representative < b.representative; }

private:
  // PRIVATE MEMBER FUNCTIONS
  std::vector<std::variant<Character, String, Literal>> analyze_pattern(const std::string& pattern);
  std::pair<int, std::size_t>                           parse_id(const std::string& pattern, const std::size_t index);
  std::pair<std::string, std::size_t> parse_literal(const std::string& pattern, const std::size_t index);

  // PRIVATE DATA
  std::string_view         representative;
  std::vector<Alternative> _alternatives;
};

#endif
