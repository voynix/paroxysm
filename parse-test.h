#ifndef ____parse_test__
#define ____parse_test__

#include "test-defs.h"
#include "parse.h"

extern const short TOKEN_STACK_SIZE;
extern const int NUM_BUILTINS;
extern const PrecedenceType DEFAULT_PRECEDENCE;
extern const ArityType DEFAULT_ARITY;

void run_parser_tests();

#endif