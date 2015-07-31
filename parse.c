#include "parse.h"

const short TOKEN_ARRAY_SIZE = 200;
const PrecedenceType DEFAULT_PRECEDENCE = 20;
const ArityType DEFAULT_ARITY = 20;

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
            return 6;
    }
    return DEFAULT_PRECEDENCE;
}

/*
 * Returns arity for each operator
 */
ArityType get_arity(BuiltinType b){
    switch(b){
        case EXPAND:
        case COLLAPSE:
        case L_PAREN:
        case R_PAREN:
            return 0;
        case INIT:
        case TERM:
        case PATH:
        case OUTN:
        case OUTC:
        case NEG:
            return 1;
        case SET:
        case AND:
        case OR:
        case LESS_THAN:
        case GREATER_THAN:
        case L_SHIFT:
        case R_SHIFT:
        case PLUS:
        case MINUS:
        case BIT_AND:
        case BIT_OR:
        case MULTIPLY:
        case DIVIDE:
            return 2;
        case BIFURC:
            return 3;
    }
    return DEFAULT_ARITY;
}

/*
 * Returns whether or not the given builtin can start a line
 */
int can_start_line(BuiltinType b){
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
            return 1;
        default:
            return 0;
    }
}

/*
 * Parse and infix expression from a token stream
 * Returns a parse tree
 */
Token parse_infix_expression(Token tokens){
    // see http://stackoverflow.com/questions/1810083/c-pointers-pointing-to-an-array-of-fixed-size
    // for explanation of this type signature
    Token (*operators)[TOKEN_ARRAY_SIZE] = malloc(sizeof(operators));
    assert(operators != NULL);
    unsigned operatorsLen = 0;
    
    Token (*output)[TOKEN_ARRAY_SIZE] = malloc(sizeof(output));
    assert(output != NULL);
    unsigned outputLen = 0;
    
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
}

/*
 * Turns a token stream into an AST
 * Returns the AST in the argument tokens
 */
void create_AST(Token* tokens){
    if(*tokens->type != BUILTIN){
        assert(0); // TODO: proper error handling
        // this should be a syntax error of some sort, probably
    }
    if(!can_start_line(*tokens->builtin)){
        assert(0); // TODO: proper error handling
        // this should also be a syntax error
    }
    Token output = *tokens; // we'll hook up the operands later, once we have them
    
    // call to parse_infix_expression here as needed, based on arity, etc
    // possibly after grabbing non-expr operands, of course
    
    tokens = output;
    
    //let's not leak memory
    free(output);
    free(operators);
}