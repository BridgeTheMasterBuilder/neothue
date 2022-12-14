bison = find_program('bison')

run_command(bison, join_paths(meson.source_root(), 'src/neothue.y'), check: true)

sources = files('main.cpp',
           'Grammar.cpp',
           'Lexer.cpp',
           'Parser.cpp',
           'terminal.cpp',
           'util.cpp',
           'neothue.tab.cc')

lexer_test_sources = files('Lexer.cpp', 'terminal.cpp', '../tests/lexer_tests.cpp')

parser_test_sources = files('Grammar.cpp', 'Lexer.cpp', 'Parser.cpp', 'terminal.cpp', '../tests/parser_tests.cpp')