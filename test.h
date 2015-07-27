#ifndef ____test__
#define ____test__

#include <stdio.h>
#include <assert.h>
#include "lex.h"

extern const unsigned int INITIAL_NAME;
extern const int EOL;
extern const char COMMENT_CHAR;
extern const int NUM_BUILTINS;
extern char* builtin_strs;

#define TEST_GROUP_INDICATOR(x) printf("Testing %s...\n", x);

#endif
