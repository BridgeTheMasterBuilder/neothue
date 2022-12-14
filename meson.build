project('nthue', 'cpp',
  version : '1.1.0',
  default_options : ['buildtype=release', 'debug=true', 'optimization=3', 'strip=true', 'werror=true', 'b_lto=true', 'cpp_rtti=true', 'warning_level=3', 'cpp_std=c++20'])

subdir('src')

executable('nthue', sources)

if get_option('test')
lexer_tests = executable('lexertest', lexer_test_sources, include_directories : 'src')
                             
parser_tests = executable('parsertest', parser_test_sources, include_directories : 'src')

test('lexer tests', lexer_tests)
test('parser tests', parser_tests)
endif