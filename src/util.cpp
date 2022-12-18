/*
    nthue, an interpreter for Neothue, a dialect of the Thue metalanguage
    Copyright (C) 2022  masterbuilder

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
#include "terminal.h"
#include <fstream>
#include <iostream>
#include <sstream>

std::string file_as_string(const std::string_view filename)
{
  const std::ifstream source_file(filename.data());
  if (!source_file) throw File_not_found(filename.data());

  std::stringstream buffer;
  buffer << source_file.rdbuf();

  return buffer.str();
}

std::size_t find_end_of_line(const std::string& source_code, const std::size_t start) noexcept
{
  std::size_t end_of_line = source_code.find('\n', start);

  if (end_of_line == std::string::npos) end_of_line = source_code.size();

  return end_of_line;
}

std::size_t find_start_of_line(const std::string& source_code, const int line_number) noexcept
{
  std::size_t start_of_line = 0;

  for (int i = 1; i < line_number; i++)
    start_of_line = source_code.find('\n', start_of_line) + 1;

  return start_of_line;
}

Config parse_command_line_options(int argc, char* argv[])
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

  return Config { classic, debug, filename, print_usage, order };
}

void report_error(const std::string_view filename,
                  const std::string&     source_code,
                  int                    line_num,
                  int                    col_num,
                  int                    length)
{
  const int         size  = source_code.size();
  const std::size_t start = find_start_of_line(source_code, line_num >= size ? size - 1 : line_num);
  const std::size_t end   = find_end_of_line(source_code, start);

  std::string line        = source_code.substr(start, end - start);

  std::cerr << bold() << filename << ':' << line_num << ':' << col_num << ": " << bold(red("error: ")) << reset();

  std::cerr
      << "Expected to find \x1B[1m=\x1B[0m but instead found a string. Note that productions must be separated by "
         "\x1B[1m=\x1B[0m\n";

  line.replace(col_num - 1, length, underline(bold(red(line.substr(col_num - 1, length)))));

  std::cerr << '\t' << line_num << " | " << line << '\n';
}

void usage() noexcept
{
  std::cerr <<
      R"(Usage: nthue [options] file

	-c, --classic		use the classic Thue dialect
	-d, --debug		print the state before and after any production application
	-h, --help		print this usage information
	-l, --left-to-right	apply productions deterministically from left-to-right
	-r, --right-to-left	apply productions deterministically from right-to-left
)";
}
