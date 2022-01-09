#ifndef MATCH_H
#define MATCH_H

#include "Grammar.h"
#include <string_view>
#include <vector>

struct Constituent {
  // CONSTRUCTORS
  Constituent(const Character& c) : type(Type::CHARACTER), id(c.value) { }
  Constituent(const Literal& c) : type(Type::LITERAL), deduced_value(c.value) { }
  Constituent(const String& c) : type(Type::STRING), id(c.value) { }
  Constituent([[maybe_unused]] const End& c) : type(Type::END), id(-1), matched(true) { }
  Constituent([[maybe_unused]] const Start& c)
      : type(Type::START), id(-1), possible_indices({ 0, 0 }), matched(true) { }

  // ENUMS
  enum class Type
  {
    START,
    CHARACTER,
    LITERAL,
    STRING,
    END
  } type;

  // PUBLIC DATA
  const int   id               = 0;
  IndexPair   possible_indices = { std::string::npos, std::string::npos };
  std::string deduced_value;
  bool        matched = false;
};

class Match {
public:
  // CONSTRUCTORS
  Match(const Alternative& alternative, const std::string_view string);

  // PUBLIC MEMBER FUNCTIONS
  IndexPair match_indices() const;

  // CONVERSION OPERATORS
  operator bool() const;

private:
  // PRIVATE MEMBER FUNCTIONS
  bool      anchored_deduce(Constituent& c1, Constituent& c2);
  bool      deduce(Constituent& c1, Constituent& c2);
  void      deduce_character_and_character(Constituent& c1, Constituent& c2);
  void      deduce_character_and_string(Constituent& c1, Constituent& c2);
  void      deduce_character_and_literal(Constituent& c1, Constituent& c2);
  void      deduce_character_on_left(Constituent& c1, Constituent& c2);
  void      deduce_character_on_right(Constituent& c1, Constituent& c2);
  void      deduce_string_and_character(Constituent& c1, Constituent& c2);
  void      deduce_string_and_string(Constituent& c1, Constituent& c2);
  void      deduce_string_and_literal(Constituent& c1, Constituent& c2);
  void      deduce_string_on_left(Constituent& c1, Constituent& c2);
  void      deduce_string_on_right(Constituent& c1, Constituent& c2);
  bool      deduce_literal(Constituent& l);
  void      deduce_literal_and_character(Constituent& c1, Constituent& c2);
  void      deduce_literal_and_string(Constituent& c1, Constituent& c2);
  void      fixed_point_anchored_deduce();
  void      fixed_point_unanchored_deduce();
  IndexPair match(const Constituent c);
  IndexPair match(const std::string_view literal);
  bool      unanchored_deduce(Constituent& c1, Constituent& c2);

  // PRIVATE DATA
  std::vector<Constituent> constituents;
  const std::string_view   string;
  std::size_t              index = 0;
};

#endif
