bison = find_program('bison')
reflex = find_program('reflex')

if bison.found()
    run_command(bison, join_paths(meson.source_root(), 'src/neothue.y'), check: true)
endif

if reflex.found()
    run_command(reflex, join_paths(meson.source_root(), 'src/neothue.l'), check: true)
endif

sources = files('main.cpp',
           'Grammar.cpp',
           'terminal.cpp',
           'util.cpp',
           'Lexer.cpp',
           'Parser.cpp',
           'ParserImplementation.cpp')

lexer_test_sources = files('Lexer.cpp', 'terminal.cpp', 'ParserImplementation.cpp', '../tests/lexer_tests.cpp')

parser_test_sources = files('Grammar.cpp', 'Lexer.cpp', 'Parser.cpp', 'terminal.cpp', 'ParserImplementation.cpp', 'util.cpp', '../tests/parser_tests.cpp')