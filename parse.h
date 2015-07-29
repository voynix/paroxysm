#ifndef ____parse__
#define ____parse__

#include <stdio.h>
#include <assert.h>
#include "lex.h"

// the max number of tokens we can process per line
const short TOKEN_ARRAY_SIZE;

void push_token_array(Token t, Token (*array)[TOKEN_ARRAY_SIZE], unsigned* arrayLength);
Token pop_token_array(Token (*array)[TOKEN_ARRAY_SIZE], unsigned* arrayLength);
void create_AST(Token* tokens);

#endif