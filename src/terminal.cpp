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

#include "terminal.h"

std::string bold(const std::string message) { return std::string("\x1B[1m") + message + "\x1B[0m"; }

std::string red() noexcept { return "\x1B[38;5;9m"; }

std::string red(const char character) { return std::string("\x1B[38;5;9m") + character + "\x1B[0m"; }

std::string red(const std::string message) { return std::string("\x1B[38;5;9m") + message + "\x1B[0m"; }

std::string bold() noexcept { return "\x1B[1m"; }

std::string reset() noexcept { return "\x1B[0m"; }

std::string underline() { return std::string("\x1B[4m"); }

std::string underline(const std::string message) { return std::string("\x1B[4m") + message + "\x1B[0m"; }
