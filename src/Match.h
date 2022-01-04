#ifndef MATCH_H
#define MATCH_H

#include "Grammar.h"
#include <string_view>
#include <vector>

struct Constituent {
  Constituent(const Character& c) : type(Type::CHARACTER), id(c.value) { }
  Constituent(const Literal& c) : type(Type::LITERAL), deduced_value(c.value) { }
  Constituent(const String& c) : type(Type::STRING), id(c.value) { }

  enum class Type
  {
    CHARACTER,
    LITERAL,
    STRING
  } type;
  const int   id = 0;
  IndexPair   possible_indices;
  std::string deduced_value;
  bool        matched = false;
};

class Match {
public:
  Match(const Alternative& alternative, const std::string_view string);
  IndexPair match_indices() const;

  operator bool() const;

private:
  IndexPair match(const Constituent c);
  IndexPair match(const std::string_view literal);

private:
  std::vector<Constituent> constituents;
  const std::string_view   string;
  std::size_t              index = 0;
};

#endif
