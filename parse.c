#include "parse.h"

// the max number of tokens we can process per line
const short TOKEN_ARRAY_SIZE = 200;
const PrecedenceType BAD_PRECEDENCE = 20;

/*
 * Push a token onto a stack
 */
void push_token_array(Token t, Token (*array)[TOKEN_ARRAY_SIZE], unsigned* arrayLength){
    assert(*arrayLength < TOKEN_ARRAY_SIZE);
    (*array)[*arrayLength] = t;
    (*arrayLength)++;
}

/*
 * Pop a token off a stack
 */
Token pop_token_array(Token (*array)[TOKEN_ARRAY_SIZE], unsigned* arrayLength){
    if(*arrayLength == 0)
        return NULL;
    (*arrayLength)--;
    return (*array)[*arrayLength];
}

/*
 * Returns precedence for each operator
 * Higher values means higher precedence
 */
PrecedenceType get_precedence(BuiltinType b){
    switch(b){
        case EXPAND:
        case COLLAPSE:
        case INIT:
        case TERM:
        case SET:
        case PATH:
        case BIFURC:
        case OUTN:
        case OUTC:
            return 0;
        case AND:
        case OR:
            return 1;
        case LESS_THAN:
        case GREATER_THAN:
        case NEG:
            return 2;
        case L_SHIFT:
        case R_SHIFT:
            return 3;
        case PLUS:
        case MINUS:
        case BIT_AND:
        case BIT_OR:
            return 4;
        case MULTIPLY:
        case DIVIDE:
            return 5;
        case L_PAREN:
        case R_PAREN:
            return BAD_PRECEDENCE;
    }
}

/*
 * Turns a token stream into an AST
 * Returns the AST in the argument tokens
 */
void create_AST(Token* tokens){
    // see http://stackoverflow.com/questions/1810083/c-pointers-pointing-to-an-array-of-fixed-size
    // for explanation of this type signature
    Token (*operators)[TOKEN_ARRAY_SIZE] = malloc(sizeof(operators));
    assert(operators != NULL);
    unsigned operatorsLen = 0;
    
    Token (*output)[TOKEN_ARRAY_SIZE] = malloc(sizeof(output));
    assert(output != NULL);
    unsigned outputLen = 0;
    
    Token nextToken = NULL;
    
    while(*tokens != NULL){
        nextToken = (*tokens)->next;
        // if it's not a builtin, stick it in output
        if((*tokens)->type != BUILTIN){
            push_token_array(*tokens, output, &outputLen);
        } else {
            
        }
        // if it's a builtin, do magic
        *tokens = nextToken;
    }
    
    tokens = output[0];
}