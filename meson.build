project('nthue', 'cpp',
  version : '1.1.0',
  default_options : ['buildtype=release', 'debug=false', 'optimization=3', 'strip=true', 'werror=true', 'b_lto=true', 'cpp_rtti=false', 'warning_level=3', 'cpp_std=c++20'])

subdir('src')

reflex = meson.get_compiler('cpp').find_library('reflex')

executable('nthue', sources, cpp_args: '-Wno-unused-but-set-variable', dependencies : [reflex])

if get_option('test')
lexer_tests = executable('lexertest', lexer_test_sources, include_directories : 'src', dependencies : [reflex])

parser_tests = executable('parsertest', parser_test_sources, include_directories : 'src', dependencies : [reflex])

test('lexer tests', lexer_tests)
test('parser tests', parser_tests)
endif