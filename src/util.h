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

#ifndef UTIL_H
#define UTIL_H

#include "Grammar.h"
#include <getopt.h>
#include <string>
#include <string_view>
#include <tuple>

// Command-line options class
struct Config {
  const bool              classic;
  const bool              debug;
  const std::string_view  filename;
  const bool              print_usage;
  const application_order order;
};

// Function prototypes
void        erase_empty_production(std::string& source_code, const std::size_t index);
void        escape_quotes(std::string& source_code);
std::string file_as_string(const std::string_view filename);
std::size_t find_end_of_line(const std::string& source_code, const std::size_t start) noexcept;
std::size_t find_start_of_line(const std::string& source_code, const int line_number) noexcept;
bool        finished_preprocessing(std::string& source_code, const std::size_t index) noexcept;
bool        maybe_erase_empty_production(std::string& source_code, const std::size_t index);
Config      parse_command_line_options(int argc, char* argv[]);
void        preprocess(std::string& source_code);
std::size_t quote_initial_state(std::string& source_code, std::size_t index);
std::size_t quote_lhs(std::string& source_code, std::size_t index);
std::size_t quote_rhs(std::string& source_code, std::size_t index);
void        report_error(const std::string_view filename, const std::string& source_code, int line_num, int col_num, int length);
std::size_t skip_blank_lines(std::string& source_code, std::size_t index) noexcept;
void        usage() noexcept;

// Exception class
struct File_not_found {
  File_not_found(const std::string file) : file(file) { }

  const std::string file;
};

#endif
