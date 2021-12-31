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

#include "util.h"
#include <iostream>
#include <sstream>

// ...⍺'β'::=γ... → ...⍺\'β\'::=γ...
void escape_quotes(std::string& source_code)
{
  std::size_t index = 0;

  // ...⍺'β'::=γ... → ...⍺'β'::=γ...
  // ^                    ^
  while ((index = source_code.find('\'', index)) != std::string::npos) {
    // ...⍺'β'::=γ... → ...⍺\'β'::=γ...
    //     ^                ^
    source_code.insert(index, "\\");
    // ...⍺\'β'::=γ... → ...⍺\'β'::=γ...
    //     ^                   ^
    index += 2;
  }
}

std::string file_as_string(const std::string_view filename)
{
  if (!filename.empty()) {
    const std::ifstream source_file(filename.data());
    if (!source_file) throw File_not_found(filename.data());

    std::stringstream buffer;
    buffer << source_file.rdbuf();

    return buffer.str();
  }
  else {
    std::string buffer;

    while (!std::cin.eof()) {
      std::string temp;
      std::cin >> temp;

      buffer += temp + ' ';
    }

    return buffer;
  }
}

// The standard doesn't say that this overload of std::string::find doesn't throw but AFAICT it never does
bool finished_preprocessing(std::string& source_code, const std::size_t index) noexcept
{
  return source_code.find("::=", index) == std::string::npos;
}

// ...\n...::=...\n⍺... → ...\n⍺...
void maybe_erase_empty_production(std::string& source_code, const std::size_t index)
{
  // ...\n...::=...\n⍺... → ...\n...::=...\n⍺...
  //                 ^         ^
  std::size_t start = source_code.rfind('\n', index) + 1;
  if (start == std::string::npos) start = 0;

  // ...\n...::=...\n⍺... → ...\n...::=...\n⍺...
  //    ^                      ~~~~~~~~~~~^
  std::size_t end = source_code.find('\n', start + 1);
  if (end == std::string::npos) end = source_code.size();

  std::size_t length = end - start;

  // ...\n...::=...\n⍺... → ...\n⍺...
  //    ~~~~~~~~~~~^           ^
  if (source_code.find_first_not_of("\t\n\r :=", start) > end) source_code.erase(start, length);
}

Options parse_command_line_options(int argc, char* argv[])
{
  bool              classic     = false;
  bool              debug       = false;
  std::string_view  filename    = "";
  bool              print_usage = false;
  application_order order       = application_order::NONDETERMINISTIC;

  const struct option options[] {
    { "classic", no_argument, 0, 'c' }, { "debug", no_argument, 0, 'd' }, { "help", no_argument, 0, 'h' },
        { "left-to-right", no_argument, 0, 'l' }, { "right-to-left", no_argument, 0, 'r' }, { 0, 0, 0, 0 },
  };

  while (true) {
    int c;

    if ((c = getopt_long(argc, argv, "cdhlr", options, NULL)) == -1) break;

    switch (c) {
      case 'c':
        classic = true;
        break;
      case 'd':
        debug = true;
        break;
      case 'h':
        print_usage = true;
        break;
      case 'l':
        order = application_order::LEFT_TO_RIGHT;
        break;
      case 'r':
        order = application_order::RIGHT_TO_LEFT;
        break;
      default:
        break;
    }
  }

  if (argv[optind]) filename = argv[optind];

  return { classic, debug, filename, print_usage, order };
}

// ⍺ → '⍺'
std::size_t quote_initial_state(std::string& source_code, std::size_t index)
{
  // ...⍺\n... → ...'⍺\n...
  //    ^         ^
  source_code.insert(index, "\'");

  // ...'⍺\n... → ...'⍺\n...
  //    ^              ^
  index = source_code.find('\n', index);
  if (index == std::string::npos) index = source_code.size();

  // ...'⍺\n... → ...'⍺'\n...
  //      ^            ^
  source_code.insert(index, "\'");

  // ...'⍺'\n... → ...'⍺'\n...
  //      ^                ^
  return index + 2;
}

// ⍺::=β → '⍺'::=β
std::size_t quote_lhs(std::string& source_code, std::size_t index)
{
  // ...⍺::=β... → ...'⍺::=β...
  //    ^             ^
  source_code.insert(index, "\'");

  // ...'⍺::=β... → ...'⍺::=β...
  //    ^                ^
  index = source_code.find("::=", index);

  // ...'⍺::=β... → ...'⍺=β...
  //      ^              ^
  source_code.replace(index, 3, "=");

  // ...'⍺::=β... → ...'⍺'=β...
  //      ^              ^
  source_code.insert(index, "\'");

  // ...'⍺'=β... → ...'⍺'=β...
  //      ^               ^
  return index + 2;
}

// ⍺::=β → ⍺::='β'
std::size_t quote_rhs(std::string& source_code, std::size_t index)
{
  // ...⍺::=β\nγ... → ...⍺::='β\nγ...
  //        ^                ^
  source_code.insert(index, "\'");

  // ...⍺::='β\nγ... → ...⍺::='β\nγ...
  //        ^                   ^
  std::size_t end = source_code.find('\n', index);
  if (end == std::string::npos) end = source_code.size();

  // ...⍺::='...~...\nγ... → ...⍺::='...~...\n'\nγ...
  //        ^                                 ^
  if (source_code.find('~', index) < end) {
    source_code.insert(end, "\n");
    index = end + 1;
  }
  // ...⍺::='β\nγ... → ...⍺::='β'\nγ...
  //          ^                 ^
  else {
    index = end;
  }

  source_code.insert(index, "\'");

  // ...⍺::='β'\nγ... → ...⍺::='β'\nγ...
  //          ^                     ^
  return index + 2;
}

// ...⍺...\n\n\n...β... → ...⍺...\n\n\n...β...
//        ^                            ^
std::size_t skip_blank_lines(std::string& source_code, std::size_t index) noexcept
{
  // ...⍺...\n\n\n...β... → ...⍺...\n\n\n...β...
  //        ^                               ^
  index = source_code.find_first_not_of("\t\n\r ", index);
  if (index == std::string::npos) index = source_code.size() - 1;

  // ...⍺...\n\n\n...β... → ...⍺...\n\n\n...β...
  //                 ^                   ^
  index = source_code.rfind('\n', index) + 1;
  if (index == std::string::npos) index = 0;

  return index;
}

void preprocess(std::string& source_code)
{
  std::size_t index = 0;

  escape_quotes(source_code);

  while (!finished_preprocessing(source_code, index)) {
    index = skip_blank_lines(source_code, index);
    index = quote_lhs(source_code, index);
    index = quote_rhs(source_code, index);
    maybe_erase_empty_production(source_code, index);
  }

  index = skip_blank_lines(source_code, index);
  quote_initial_state(source_code, index);
}

void usage() noexcept
{
  std::cerr <<
      R"(Usage: thue [options] file

	-c, --classic		use the classic Thue dialect
	-d, --debug		print the state before and after any production application
	-h, --help		print this usage information
	-l, --left-to-right	apply productions deterministically from left-to-right
	-r, --right-to-left	apply productions deterministically from right-to-left
)";
}
