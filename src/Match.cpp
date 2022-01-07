#include "Match.h"
#include <iostream>
#include <iterator>

Match::Match(const Alternative& alternative, const std::string_view string) : string(string)
{
  for (auto& c : alternative) {
    Constituent constituent = std::visit([](const auto& constituent) { return Constituent(constituent); }, c);

    constituents.push_back(constituent);
  }

  bool matched                  = true;
  int  last_number_of_unmatched = 0;
  bool changing                 = false;

  do {
    int number_of_unmatched = 0;

    for (auto c1 = constituents.begin(), c2 = constituents.begin() + 1; c2 != constituents.end(); c1++, c2++) {
      Constituent& first_constituent  = *c1;
      Constituent& second_constituent = *c2;

      if (!deduce(first_constituent, second_constituent)) {
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

    index = 0;
  } while (changing);
}

IndexPair Match::match_indices() const
{
  return { constituents[0].possible_indices.first, constituents[constituents.size() - 1].possible_indices.second };
}

Match::operator bool() const
{
  for (const auto& constituent : constituents)
    if (constituent.matched == false) return false;

  return true;
}

// TODO finish this
bool Match::deduce(Constituent& c1, Constituent& c2)
{
  switch (c1.type) {
    case Constituent::Type::CHARACTER:
      switch (c2.type) {
        case Constituent::Type::CHARACTER:
          deduce_characters(c1, c2);
          break;
        case Constituent::Type::LITERAL:
          deduce_character_and_literal(c1, c2);
          break;
        case Constituent::Type::STRING:
          deduce_character_and_string(c1, c2);
          break;
      }
      break;
    case Constituent::Type::LITERAL:
      switch (c2.type) {
        case Constituent::Type::CHARACTER:
          deduce_literal_and_character(c1, c2);
          break;
        case Constituent::Type::STRING:
          break;
        default:
          break;
      }
      break;
    case Constituent::Type::STRING:
      switch (c2.type) {
        case Constituent::Type::CHARACTER:
          break;
        case Constituent::Type::LITERAL:
          break;
        case Constituent::Type::STRING:
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

void Match::deduce_character_on_left(Constituent& c1, Constituent& c2)
{
  const auto [start, end] = c2.possible_indices;

  c1.possible_indices     = { start - 1, start };
  c1.deduced_value        = string.substr(start - 1, 1);
  c1.matched              = true;

  std::cout << "Deduced character c" << c1.id << " to be " << c1.deduced_value << '\n';
}

void Match::deduce_character_on_right(Constituent& c1, Constituent& c2)
{
  const auto [start, end] = c1.possible_indices;

  c2.possible_indices     = { end, end + 1 };
  c2.deduced_value        = string.substr(end, 1);
  if (!c2.deduced_value.empty()) {
    c2.matched = true;
    std::cout << "Deduced character c" << c2.id << " to be " << c2.deduced_value << '\n';
  }
}

void Match::deduce_characters(Constituent& c1, Constituent& c2)
{
  if (c1.matched && c2.matched) return;
  else if (!c1.matched && c2.matched) {
    deduce_character_on_left(c1, c2);
  }
  else if (c1.matched && !c2.matched) {
    deduce_character_on_right(c1, c2);
  }
  else {
    c1.possible_indices     = { index, string.size() };
    c2.possible_indices     = { index + 1, string.size() };

    const auto [start, end] = c2.possible_indices;

    if ((end - start) == 1) {
      c2.deduced_value = string[start];
      c2.matched       = true;
      deduce_character_on_left(c1, c2);
    }
  }

  index++;
}

void Match::deduce_character_and_literal(Constituent& c1, Constituent& c2)
{
  if (!c2.matched) {
    const std::string_view literal = c2.deduced_value;

    std::size_t start              = string.find(literal, index);
    if (start == std::string::npos) return;

    const std::size_t end = start + literal.size();
    // if (end) c2.possible_indices = { start, end };
    c2.possible_indices   = { start, end };
    c2.matched            = true;

    // c1.possible_indices = { start - 1, start };
    // c1.deduced_value    = string.substr(start - 1, 1);
    // c1.matched          = true;
    deduce_character_on_left(c1, c2);

    index++;
  }
}

void Match::deduce_literal_and_character(Constituent& c1, Constituent& c2)
{
  if (!c2.matched) {
    const std::string_view literal = c1.deduced_value;

    const std::size_t start        = string.find(literal, index);
    const std::size_t end          = start + literal.size();
    c1.possible_indices            = { start, end };
    c1.matched                     = true;

    c2.possible_indices            = { end, end + 1 };
    c2.deduced_value               = string.substr(end, 1);
    c2.matched                     = true;

    index                          = end;
  }
}

void Match::deduce_character_and_string(Constituent& c1, Constituent& s)
{
  if (c1.matched && s.matched) return;
  else {
    const auto [start, end]   = c1.possible_indices;
    const auto [start2, end2] = s.possible_indices;

    if (start2 == std::string::npos) s.possible_indices = { start + 1, string.size() };
    else s.possible_indices = { start + 1, end2 };

    s.deduced_value = string.substr(start + 1, end2 - end);
    s.matched       = true;

    deduce_character_on_left(c1, s);
  }

  index++;
}

void Match::deduce_string_on_left(Constituent& c1, Constituent& c2)
{
  const auto [start, end]   = c1.possible_indices;
  const auto [start2, end2] = c2.possible_indices;

  c2.possible_indices       = { start, start2 - 1 };
  c2.deduced_value          = string.substr(start, start2 - start);
  c2.matched                = true;

  std::cout << "Deduced string s" << c1.id << " to be " << c1.deduced_value << '\n';
}

void Match::deduce_string_on_right(Constituent& c1, Constituent& c2)
{
  const auto [start, end] = c1.possible_indices;

  c2.possible_indices     = { end, string.size() };
  c2.deduced_value        = string.substr(end, string.size() - end);
  c2.matched              = true;

  std::cout << "Deduced string s" << c2.id << " to be " << c2.deduced_value << '\n';
}

// void Match::deduce_string_and_character(Constituent& c1, Constituent& c2)
// {
//   if (c1.matched && c2.matched) return;
//   else if (!c1.matched && c2.matched) {
//     const auto [start, end] = c2.possible_indices;

//     c1.possible_indices     = { start - 1, start };
//     c1.deduced_value        = string.substr(start - 1, 1);
//     c1.matched              = true;
//     std::cout << "Character c" << c1.id << " has the value " << string[start - 1] << '\n';
//   }
//   else if (c1.matched && !c2.matched) {
//     const auto [start, end] = c1.possible_indices;

//     if (c2.possible_indices.first == std::string::npos) {
//       c2.possible_indices = { end, string.size() };
//     }
//     else {
//       const auto [start2, end2] = c2.possible_indices;
//       c2.deduced_value          = string.substr(end, end2 - end);
//       c2.matched                = true;
//       std::cout << "String s" << c2.id << " has the value " << c2.deduced_value << '\n';
//     }
//   }
//   else {
//     std::cout << "Trying to deduce what character c" << c1.id << " is...\n";
//     c1.possible_indices     = { index, string.size() };

//     c2.possible_indices     = { index + 1, string.size() };

//     const auto [start, end] = c1.possible_indices;
//     std::cout << "It's located between indices " << start << "-" << end << '\n';

//     std::cout << "Trying to deduce what string s" << c2.id << " is...\n";

//     const auto [start2, end2] = c2.possible_indices;
//     std::cout << "It could be the string " << string.substr(start2) << '\n';
//   }

//   index++;
// }

IndexPair Match::match(const Constituent c) { return match(c.deduced_value); }

IndexPair Match::match(const std::string_view literal)
{
  std::size_t index_of_match = string.find(literal);

  if (index_of_match == std::string::npos) return { std::string::npos, std::string::npos };
  else return { index_of_match, index_of_match + literal.size() };
}
