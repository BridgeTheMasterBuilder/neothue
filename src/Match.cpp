#include "Match.h"
#include <iostream>

/***************
 * CONSTRUCTORS *
 ***************/
Match::Match(const Alternative& alternative, const std::string_view string) : string(string)
{
  for (const auto& constituent : alternative)
    std::visit([this](const auto& constituent) { return constituents.push_back(Constituent(constituent)); },
               constituent);

  constituents[constituents.size() - 1].possible_indices = { string.size(), string.size() };

  bool contains_anchor                                   = false;

  for (auto& constituent : constituents)
    if (deduce_literal(constituent)) contains_anchor = true;

  if (contains_anchor) fixed_point_anchored_deduce();
  else fixed_point_unanchored_deduce();
}

/**************************
 * PUBLIC MEMBER FUNCTIONS *
 **************************/
IndexPair Match::match_indices() const
{
  return { constituents[1].possible_indices.first, constituents[constituents.size() - 2].possible_indices.second };
}

/***********************
 * CONVERSION OPERATORS *
 ***********************/
Match::operator bool() const
{
  for (const auto& constituent : constituents)
    if (constituent.matched == false) return false;

  return true;
}

/***************************
 * PRIVATE MEMBER FUNCTIONS *
 ***************************/
bool Match::anchored_deduce(Constituent& c1, Constituent& c2)
{
  switch (c1.type) {
    case Constituent::Type::CHARACTER:
      switch (c2.type) {
        case Constituent::Type::CHARACTER:
          deduce_character_and_character(c1, c2);
          break;
        case Constituent::Type::STRING:
          deduce_character_and_string(c1, c2);
          break;
        case Constituent::Type::LITERAL:
          deduce_character_and_literal(c1, c2);
          break;
        default:
          break;
      }
      break;
    case Constituent::Type::STRING:
      switch (c2.type) {
        case Constituent::Type::CHARACTER:
          deduce_string_and_character(c1, c2);
          break;
        case Constituent::Type::LITERAL:
          deduce_string_and_literal(c1, c2);
          break;
        case Constituent::Type::END:
          if (!c1.matched) deduce_string_on_left(c1, c2);
          break;
        default:
          break;
      }
      break;
    case Constituent::Type::LITERAL:
      switch (c2.type) {
        case Constituent::Type::CHARACTER:
          deduce_literal_and_character(c1, c2);
          break;
        case Constituent::Type::STRING:
          deduce_literal_and_string(c1, c2);
          break;
        default:
          break;
      }
      break;
    case Constituent::Type::START:
      switch (c2.type) {
        case Constituent::Type::STRING:
          if (!c2.matched) deduce_string_on_right(c1, c2);
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }

  return c1.matched && c2.matched;
}

bool Match::deduce_literal(Constituent& l)
{
  if (l.type != Constituent::Type::LITERAL) return false;

  const std::string_view literal = l.deduced_value;

  const std::size_t start        = string.find(literal, index);
  if (start == std::string::npos) return false;

  const std::size_t end = start + literal.size();

  l.possible_indices    = { start, end };
  l.matched             = true;

  index                 = end;

  return true;
}

void Match::deduce_character_and_character(Constituent& c1, Constituent& c2)
{
  if (!c1.matched && c2.matched) deduce_character_on_left(c1, c2);
  else if (c1.matched && !c2.matched) deduce_character_on_right(c1, c2);

  index++;
}

void Match::deduce_character_and_string(Constituent& c, Constituent& s)
{
  if (!c.matched && s.matched) deduce_character_on_left(c, s);
  else if (c.matched && !s.matched) deduce_string_on_right(c, s);

  index++;
}

void Match::deduce_character_and_literal(Constituent& c, Constituent& l)
{
  if (c.matched && l.matched) return;

  if (!l.matched) {
    const std::string_view literal = l.deduced_value;

    const std::size_t start        = string.find(literal, index);
    if (start == std::string::npos) return;

    std::size_t end = start + literal.size();
    if (end >= string.size()) end = string.size();

    l.possible_indices = { start, end };
    l.matched          = true;
  }

  deduce_character_on_left(c, l);

  index++;
}

void Match::deduce_character_on_left(Constituent& c1, Constituent& c2)
{
  const auto [start, end]   = c2.possible_indices;

  c1.possible_indices       = { start - 1, start };

  const auto [start2, end2] = c1.possible_indices;

  c1.deduced_value          = string[start2];
  c1.matched                = true;

  std::cout << "Deduced character c" << c1.id << " to be " << c1.deduced_value << '\n';
}

void Match::deduce_character_on_right(Constituent& c1, Constituent& c2)
{
  const auto [start, end]   = c1.possible_indices;

  c2.possible_indices       = { end, end + 1 };

  const auto [start2, end2] = c2.possible_indices;

  c2.deduced_value          = string[start2];
  c2.matched                = true;

  std::cout << "Deduced character c" << c2.id << " to be " << c2.deduced_value << '\n';
}

void Match::deduce_string_and_character(Constituent& s, Constituent& c)
{
  if (s.matched && c.matched) return;
  else if (s.matched) deduce_character_on_right(s, c);
  else if (c.matched) deduce_string_on_left(s, c);

  index++;
}

void Match::deduce_string_and_string([[maybe_unused]] Constituent& c1, [[maybe_unused]] Constituent& c2) { return; }

void Match::deduce_string_and_literal(Constituent& s, Constituent& l)
{
  if (!l.matched) {
    const std::string_view literal = l.deduced_value;

    const std::size_t start        = string.find(literal, index);
    if (start == std::string::npos) return;

    std::size_t end = start + literal.size();
    if (end >= string.size()) end = string.size();

    l.possible_indices = { start, end };
    l.matched          = true;

    deduce_string_on_left(s, l);

    index = end;
  }
  else deduce_string_on_left(s, l);
}

void Match::deduce_string_on_left(Constituent& s, Constituent& c2)
{
  const auto [start, end]   = s.possible_indices;
  const auto [start2, end2] = c2.possible_indices;

  s.possible_indices        = { start, start2 };

  if (start == std::string::npos) return;
  // else if (index == 0) {
  else {
    // s.possible_indices = { index, start2 };
    s.deduced_value = string.substr(start, start2 - start);
    s.matched       = true;

    std::cout << "Deduced string s" << s.id << " to be " << s.deduced_value << '\n';
  }
  // else if (start2 == string.size()) {
  //   s.possible_indices = { start, start2 };
  //   s.deduced_value    = string.substr(start, start2 - start);
  //   s.matched          = true;

  //   std::cout << "Deduced string s" << s.id << " to be " << s.deduced_value << '\n';
  // }
}

void Match::deduce_string_on_right(Constituent& c1, Constituent& s)
{
  const auto [start, end]   = c1.possible_indices;
  const auto [start2, end2] = s.possible_indices;

  s.possible_indices        = { end, end2 };

  if (end2 != std::string::npos) {
    s.deduced_value = string.substr(end, end2 - end);
    s.matched       = true;

    std::cout << "Deduced string s" << s.id << " to be " << s.deduced_value << '\n';
  }
}

void Match::deduce_literal_and_character(Constituent& l, Constituent& c)
{
  if (l.matched && c.matched) return;

  if (!l.matched) {
    const std::string_view literal = l.deduced_value;

    const std::size_t start        = string.find(literal, index);
    if (start == std::string::npos) return;

    std::size_t end = start + literal.size();
    if (end >= string.size()) end = string.size();

    l.possible_indices = { start, end };
    l.matched          = true;

    index              = end;
  }

  deduce_character_on_right(l, c);
}

void Match::deduce_literal_and_string(Constituent& l, Constituent& s)
{
  if (!l.matched) {
    const std::string_view literal = l.deduced_value;

    const std::size_t start        = string.find(literal, index);
    if (start == std::string::npos) return;

    std::size_t end = start + literal.size();
    if (end >= string.size()) end = string.size();

    l.possible_indices = { start, end };
    l.matched          = true;

    index              = end;
  }

  deduce_string_on_right(l, s);
}

void Match::fixed_point_anchored_deduce()
{
  bool changing                 = false;
  int  last_number_of_unmatched = 0;
  bool matched                  = false;

  do {
    index                   = 0;
    int number_of_unmatched = 0;

    for (auto c1 = constituents.begin(), c2 = constituents.begin() + 1; c2 != constituents.end(); c1++, c2++) {
      Constituent& first_constituent  = *c1;
      Constituent& second_constituent = *c2;

      if (!anchored_deduce(first_constituent, second_constituent)) {
        matched = false;
        number_of_unmatched++;
      }
    }

    if (matched) break;

    if (last_number_of_unmatched != number_of_unmatched) {
      changing                 = true;
      last_number_of_unmatched = number_of_unmatched;
    }
    else changing = false;
  } while (changing);
}

void Match::fixed_point_unanchored_deduce()
{
  bool changing                 = false;
  int  last_number_of_unmatched = 0;
  bool matched                  = false;

  do {
    index                   = 0;
    int number_of_unmatched = 0;

    for (auto c1 = constituents.begin(), c2 = constituents.begin() + 1; c2 != constituents.end(); c1++, c2++) {
      Constituent& first_constituent  = *c1;
      Constituent& second_constituent = *c2;

      if (!unanchored_deduce(first_constituent, second_constituent)) {
        matched = false;
        number_of_unmatched++;
      }
    }

    if (matched) break;

    if (last_number_of_unmatched != number_of_unmatched) {
      changing                 = true;
      last_number_of_unmatched = number_of_unmatched;
    }
    else changing = false;
  } while (changing);
}

IndexPair Match::match(const Constituent c) { return match(c.deduced_value); }

IndexPair Match::match(const std::string_view literal)
{
  std::size_t index_of_match = string.find(literal);

  if (index_of_match == std::string::npos) return { std::string::npos, std::string::npos };
  else return { index_of_match, index_of_match + literal.size() };
}

bool Match::unanchored_deduce(Constituent& c1, Constituent& c2)
{
  switch (c1.type) {
    case Constituent::Type::CHARACTER:
      switch (c2.type) {
        case Constituent::Type::CHARACTER:
          deduce_character_and_character(c1, c2);
          break;
        case Constituent::Type::STRING:
          deduce_character_and_string(c1, c2);
          break;
        case Constituent::Type::LITERAL:
          deduce_character_and_literal(c1, c2);
          break;
        case Constituent::Type::END:
          if (!c1.matched) deduce_character_on_left(c1, c2);
          break;
        default:
          break;
      }
      break;
    case Constituent::Type::STRING:
      switch (c2.type) {
        case Constituent::Type::CHARACTER:
          deduce_string_and_character(c1, c2);
          break;
        case Constituent::Type::END:
          if (!c1.matched) deduce_string_on_left(c1, c2);
          break;
        default:
          break;
      }
      break;
    case Constituent::Type::LITERAL:
      switch (c2.type) {
        case Constituent::Type::CHARACTER:
          deduce_literal_and_character(c1, c2);
          break;
        default:
          break;
      }
      break;
    case Constituent::Type::START:
      switch (c2.type) {
        case Constituent::Type::STRING:
          if (!c2.matched) deduce_string_on_right(c1, c2);
          break;
        case Constituent::Type::CHARACTER:
          if (!c2.matched) deduce_character_on_right(c1, c2);
          break;
        case Constituent::Type::END:
          return false;
        default:
          break;
      }
      break;
    default:
      break;
  }

  return c1.matched && c2.matched;
}
