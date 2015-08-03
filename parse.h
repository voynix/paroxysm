#ifndef ____parse__
#define ____parse__

#include <stdio.h>
#include <assert.h>
#include "lex.h"

typedef unsigned short PrecedenceType;
typedef unsigned short ArityType;

// the max number of tokens we can process per line
const short TOKEN_STACK_SIZE;

int get_token_length(Token t);

void push_token_stack(Token t, Token (*stack)[TOKEN_STACK_SIZE], unsigned* stackLength);
Token pop_token_stack(Token (*stack)[TOKEN_STACK_SIZE], unsigned* stackLength);

PrecedenceType get_precedence(BuiltinType b);
ArityType get_arity(BuiltinType b);
int can_start_line(BuiltinType b);

Token parse_infix_expression(Token tokens);
void create_AST(Token* tokens);

#endif