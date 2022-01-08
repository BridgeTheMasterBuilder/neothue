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
  const int   id               = 0;
  IndexPair   possible_indices = { std::string::npos, std::string::npos };
  std::string deduced_value;
  bool        matched = false;
};

class Match {
public:
  Match(const Alternative& alternative, const std::string_view string);
  IndexPair match_indices() const;

  operator bool() const;

private:
  bool      deduce(Constituent& c1, Constituent& c2);
  void      deduce_literal_and_string(Constituent& c1, Constituent& c2);
  void      deduce_string_and_literal(Constituent& c1, Constituent& c2);
  void      deduce_strings(Constituent& c1, Constituent& c2);
  void      deduce_character_on_left(Constituent& c1, Constituent& c2);
  void      deduce_character_on_right(Constituent& c1, Constituent& c2);
  void      deduce_string_on_left(Constituent& c1, Constituent& c2);
  void      deduce_string_on_right(Constituent& c1, Constituent& c2);
  void      deduce_characters(Constituent& c1, Constituent& c2);
  void      deduce_character_and_literal(Constituent& c1, Constituent& c2);
  void      deduce_literal_and_character(Constituent& c1, Constituent& c2);
  void      deduce_character_and_string(Constituent& c1, Constituent& c2);
  void      deduce_string_and_character(Constituent& c1, Constituent& c2);
  IndexPair match(const Constituent c);
  IndexPair match(const std::string_view literal);

private:
  std::vector<Constituent> constituents;
  const std::string_view   string;
  std::size_t              index = 0;
};

#endif
