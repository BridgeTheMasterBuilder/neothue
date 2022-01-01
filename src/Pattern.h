#ifndef PATTERN_H
#define PATTERN_H

#include <string>
#include <string_view>

class Pattern {
public:
  Pattern(const std::string_view pattern) : pattern(pattern) { }

  friend bool operator<(const Pattern& a, const Pattern& b) { return a.pattern < b.pattern; }

private:
  std::string pattern;
};

#endif
