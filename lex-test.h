#ifndef ____lex_test__
#define ____lex_test__

#include "test-defs.h"
#include "lex.h"

extern const unsigned INITIAL_NAME;
extern const int EOL;
extern const char COMMENT_CHAR;
extern const int NUM_BUILTINS;
extern char* builtin_strs[];

void run_lexer_tests();

#endif
