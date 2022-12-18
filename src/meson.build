python = find_program('python')
bison = find_program('bison')
reflex = find_program('reflex')

if python.found()
    run_command(python, join_paths(meson.source_root(), 'src/generate_lexer.py'), 'neothue', join_paths(meson.source_root(), 'src/neothue.syntax'), check: true)
    run_command(python, join_paths(meson.source_root(), 'src/generate_lexer.py'), 'thue', join_paths(meson.source_root(), 'src/thue.syntax'), check: true)
    run_command(python, join_paths(meson.source_root(), 'src/generate_parser.py'), 'neothue', join_paths(meson.source_root(), 'src/neothue.grammar'), check: true)
    run_command(python, join_paths(meson.source_root(), 'src/generate_parser.py'), 'thue', join_paths(meson.source_root(), 'src/thue.grammar'), check: true)

    if reflex.found()
        run_command(reflex, join_paths(meson.source_root(), 'src/generated/neothue.l'), check: true)
        run_command(reflex, join_paths(meson.source_root(), 'src/generated/thue.l'), check: true)
    endif

    if bison.found()
        run_command(bison, join_paths(meson.source_root(), 'src/generated/neothue.y'), check: true)
        run_command(bison, join_paths(meson.source_root(), 'src/generated/thue.y'), check: true)
    endif
endif

sources = files('main.cpp',
           'Grammar.cpp',
           'terminal.cpp',
           'util.cpp',
           'generated/ThueLexer.cpp',
           'generated/NeothueLexer.cpp',
           'generated/NeothueParserImplementation.cpp',
           'generated/ThueParserImplementation.cpp')

lexer_test_sources = files('generated/ThueLexer.cpp', 'generated/NeothueLexer.cpp', 'util.cpp', 'terminal.cpp', 'generated/NeothueParserImplementation.cpp', 'generated/ThueParserImplementation.cpp', '../tests/lexer_tests.cpp')

parser_test_sources = files('Grammar.cpp', 'generated/ThueLexer.cpp', 'generated/NeothueLexer.cpp', 'util.cpp', 'terminal.cpp', 'generated/NeothueParserImplementation.cpp', 'generated/ThueParserImplementation.cpp', 'util.cpp', '../tests/parser_tests.cpp')