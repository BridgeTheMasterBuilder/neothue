# Introduction

Thue is an [esoteric programming language (esolang)](https://esolangs.org/wiki/Esoteric_programming_language) created by John Colagioia in 2000. It is based around [string rewriting](https://en.wikipedia.org/wiki/Semi-Thue_system) (string rewriting systems are also known as Semi-Thue systems, named after the Norwegian mathematician Axel Thue, which is also this language's namesake). Semi-Thue systems are essentially equivalent to Chomsky's [type-0 grammars](https://en.wikipedia.org/wiki/Unrestricted_grammar) which are in turn essentially equivalent to Turing machines. Thus, Thue is Turing complete. Personally I find Thue very interesting because it's like the perfect mix between Turing machines and lambda calculus. This implementation completely overhauls the syntax and clarifies semantic ambiguities in the original.

The following options are supported:

```
-c, --classic		use the classic Thue dialect
-d, --debug		print the state before and after any production application
-h, --help		print this usage information
-l, --left-to-right	apply productions deterministically from left-to-right
-r, --right-to-left	apply productions deterministically from right-to-left
```
The `examples` directory contains Thue programs distributed by the original author with the original implementation. They can be run using the `--classic` flag. Currently the only program written in the new syntax distributed with this implementation is a simple Hello World, however more programs may be added later.

# Build instructions
This implementation of Thue depends on `GNU getopt` as it uses `getopt_long` (but non-GNU implementations of `getopt` that have `getopt_long` may also work - for instance FreeBSD). In addition, a C++ compiler that supports C++20 is required (this program compiles with both GCC 11.3.0 and Clang 14.0.4).

Enclosed is a Meson build description which can be used to build Thue. To use Meson to build the program simply run the commands

```
meson setup <build directory>
meson compile -C <build directory>
```

Which will build the program and place it in `<build directory>`.

To build and run tests, do

```
meson configure -Dtest=true <build directory>
meson compile -C <build directory>
meson test -C <build directory>
```

# Running the interpreter
```
thue [options] file
```

If any arguments remain after processing command-line options, the first non-option argument is taken to be a filename and Thue source code is read from that file and the rest of the arguments are ignored.

**Note:** Options may come before or after the filename

# Syntax and semantics
Thue's lexical grammar is extremely simple. There are only two types of lexemes: strings and the separator symbol "=". Strings may be quoted; both single quotes and double quotes are supported but they must be symmetrical. The following is the complete lexical syntax in EBNF notation:

```
string = "'", { symbol - "'" }, "'"
	| '"', { symbol - '"' }, '"'
	| { symbol } ;
symbol = ? any character ? ;
separator = '=' ;
comment = ';', { symbol - '\n' } ;
```

(`{ symbol - "'" }` should be understood to mean that strings that start with a single quote may not contain *unescaped* single quotes) 

which is also described by the regular expressions `/'[^']*'|"[^"]*"|[^'"].*/` and `/=/`.

**Note 1:** If the first character of a string is not ' or " then quotes within strings have no special meaning at all. Quotes may also be escaped with \ to disable quoting.

**Note 2:** Whitespace is not significant except that two productions must be separated by whitespace if the right hand side of the first production and the left hand side of the second production are both unquoted. The empty string is valid on either side of a production, but the fact that whitespace is not significant means that productions are not newline-terminated. Thus, the following program:

```
α=β
β=

α
```

is equivalent to

```
α=β
β=α
```

That is, the former does not mean that the second production rewrites β to the empty string. In order to have empty right hand sides the initial state (i.e. whatever follows the last production) must be empty or the empty string must be quoted.

**Note 3:** The input operator `:::` reads a string from standard input and inserts it at the same position (the three colons are then erased). 

**Note 4:** The output operator `~` outputs everything to its right to standard output. Because both the input and output operators behave exactly like regular productions, if the output operator appears in the initial state of the program it is only applied once (applying this production effectively consumes the output operator and erases it from the initial state). This is done in the interest of making it not completely unusable, because leaving the output operator in the initial state means that it will be continuously applied. However, if the output operator appears on the right hand side of a production, it is not consumed, which allows productions to have side effects which are triggered whenever they are applied.

**Note 5:** In Thue, both of these operators may be redefined by the user. Thus, they do not have their normal behavior when they appear on the left hand side of a production.

**Note 6:** Comments start with a semicolon (`;`) and extend to the end of the current line. Comments may appear anywhere except inside quoted strings.

---

The following is a complete overview of Thue's grammar (previously defined elements have been omitted):

```
program = { production }, [ initial state ] ;
production = string, separator, string ;
initial state = string ;
```

Which is actually a regular language, which is very ironic considering that Thue itself is a language for working with unrestricted grammars. It is described by the regular expression 

`/(('[^']*'|"[^"]*"|[^'"].*)=('[^']*'|"[^"]*"|[^'"].*) )*('[^']*'|"[^"]*"|[^'"].*)?/`.

(The above RE is not in any particular syntax. `.` really does mean *any* character, including newline. Also, all unnecessary whitespace is omitted, so this regular expression does not represent all possible strings which constitute a valid Thue program)

The grammar is extremely loose. The only pattern that constitutes an illegal Thue program is two adjacent strings in a context where a production definition is expected. Everything else is legal, including the empty program as well as a series of ε=ε productions that look like a series of separators if whitespace is omitted: ===.

**Note 1:** Productions with the empty string as their left hand side will never fail to match. They will even match an empty initial state. This makes such productions impractical because it means that any program that contains such a production will never terminate. However, it can be used to implement a kind of non-ending version of the UNIX program `yes`:

```
=~:::
```

**Note 2:** The quoting style of left and right hand sides of a production need not be identical.

# Differences from original
- This implementation uses EBNF-like syntax with = as a separator instead of ::=.
- The original language had no concept of quoting which placed certain restrictions on strings. For example, the left hand side of a production cannot contain ::=, because that would be interpreted as a production separator. Moreover, strings cannot contain newline characters and are also also implicitly terminated by a newline. The ability to quote allows strings to be truly arbitrary.
- Treatment of whitespace differs. In this implementation, whitespace is completely ignored apart from acting as a delimiter. Thus, unquoted strings cannot contain whitespace.
- This implementation does not use a terminating "empty production" to mark the end of the grammar as it is unnecessary. One could also argue that it is not consistent with the rest of the language, because a production with an empty left hand side semantically entails an infinite loop. 

However, if you pass the flag `-c, --classic` then Thue programs written in the original syntax are accepted and behave as they do in the original. The only exception is that the two implementations are nondeterministic in different ways because different pseudorandom algorithms are used. Nondeterminism in this implementation also differs slightly in the way it's handled. After the productions have been randomly shuffled they are applied one by one scanning the shuffled productions from top to bottom. However, they are also applied in a random fashion, either starting from the left side of the string or the right.

For more information consult doc/README.md which is the original specification of the Thue language.

Out of respect for the original author, this implementation is covered by the GPLv3 license, same as the original implementation (even though it uses none of the original code).

# Reporting bugs
If you come across any bugs I would be very grateful if you would let me know by opening an issue here on GitHub. Here are some things I consider to be bugs:

- Any deviation whatsoever from the above specification in the behavior of the interpreter.
- Old Thue programs exhibiting divergent behavior when run under this implementation with the `--classic` flag. The only exception being different behavior in nondeterministic programs as their behavior is essentially undefined.
- Unclear or ugly source code. I believe that code is meant to be read, if anything is unclear please let me know.
- Any bugs or performance issues.
