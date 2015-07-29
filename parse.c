#include "parse.h"

// the max number of tokens we can process per line
const short TOKEN_ARRAY_SIZE = 200;

void push_token_array(Token t, Token (*array)[TOKEN_ARRAY_SIZE], unsigned* arrayLength){
    if(*arrayLength >= TOKEN_ARRAY_SIZE){
        assert(0); // TODO: actual error handling
    }
    (*array)[*arrayLength] = t;
    (*arrayLength)++;
}

Token pop_token_array(Token (*array)[TOKEN_ARRAY_SIZE], unsigned* arrayLength){
    if(*arrayLength == 0)
        return NULL;
    (*arrayLength)--;
    return (*array)[*arrayLength];
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