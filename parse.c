#include "parse.h"

const short TOKEN_STACK_SIZE = 200;
const PrecedenceType DEFAULT_PRECEDENCE = 20;
const ArityType DEFAULT_ARITY = 20;

/*
 * Returns the length of token stream t
 */
int get_token_length(Token t){
    int length = 0;
    while(t != NULL){
        t = t->next;
        length++;
    }
    return length;
}

/*
 * Push a token onto a stack
 */
void push_token_stack(Token t, Token (*stack)[TOKEN_STACK_SIZE], unsigned* stackLength){
    assert(*stackLength < TOKEN_STACK_SIZE);
    (*stack)[*stackLength] = t;
    (*stackLength)++;
}

/*
 * Pop a token off a stack
 */
Token pop_token_stack(Token (*stack)[TOKEN_STACK_SIZE], unsigned* stackLength){
    if(*stackLength == 0)
        return NULL;
    (*stackLength)--;
    return (*stack)[*stackLength];
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
 * Takes the top operator off of operators, grabs operands from output
 * and puts the new tree on output
 */
void pop_operator(Token (*operators)[TOKEN_STACK_SIZE], unsigned* operatorsLen, Token (*output)[TOKEN_STACK_SIZE], unsigned* outputLen){
    Token op = pop_token_stack(operators, operatorsLen);
    if(get_arity(op->builtin) == 1){
        if(*outputLen < 1){
            assert(0); // TODO: proper error handling
        }
        op->left = pop_token_stack(output, outputLen);
    } else { // all the other infix ops are binary
        assert(get_arity(op->builtin) == 2);
        if(*outputLen < 2){
            assert(0); // TODO: proper error handling
        }
        // stack, so fill in operands in reverse order
        op->right = pop_token_stack(output, outputLen);
        op->left = pop_token_stack(output, outputLen);
    }
    push_token_stack(op, output, outputLen);
}

/*
 * Parse and infix expression from a token stream
 * Returns a parse tree
 * See https://en.wikipedia.org/wiki/Shunting-yard_algorithm
 */
Token parse_infix_expression(Token tokens){
    // see http://stackoverflow.com/questions/1810083/c-pointers-pointing-to-an-array-of-fixed-size
    // for explanation of this type signature
    Token (*operators)[TOKEN_STACK_SIZE] = malloc(sizeof(operators));
    assert(operators != NULL);
    unsigned operatorsLen = 0;
    
    Token (*output)[TOKEN_STACK_SIZE] = malloc(sizeof(output));
    assert(output != NULL);
    unsigned outputLen = 0;
    
    Token nextToken;
    while(tokens != NULL){
        nextToken = tokens->next;
        // if it's not a builtin, stick it in output
        if(tokens->type != BUILTIN){
            push_token_stack(tokens, output, &outputLen);
        } else { // if it's a builtin, do magic
            // check if it's allowed to be here
            if(can_start_line(tokens->builtin)){
                assert(0); // TODO: proper error handling
            }
            if(operatorsLen == 0){
                push_token_stack(tokens, operators, &operatorsLen);
            } else if(tokens->builtin == L_PAREN){
                push_token_stack(tokens, operators, &operatorsLen);
            } else if(tokens->builtin == R_PAREN){
                // pop operators until L_PAREN
                free_token(&tokens); // we don't need the R_PAREN
                while((*operators)[operatorsLen - 1]->builtin != L_PAREN && operatorsLen > 0)
                    pop_operator(operators, &operatorsLen, output, &outputLen);
                if(operatorsLen == 0){ // there was no L_PAREN, so error
                    assert(0); // TODO: proper error handling
                } else {
                    // discard the L_PAREN
                    Token discard = pop_token_stack(operators, &operatorsLen);
                    free_token(&discard);
                }
            } else if(get_precedence(tokens->builtin) > get_precedence((*operators)[0]->builtin)){
                push_token_stack(tokens, operators, &operatorsLen);
            } else {
                // we are not a parenthesis and we're lower precedence than
                // the operator on the stack, so pop it and put us there
                pop_operator(operators, &operatorsLen, output, &outputLen);
                push_token_stack(tokens, operators, &operatorsLen);
            }
        }
        tokens = nextToken;
    }
    
    // pop all remaining operators
    while(operatorsLen > 0)
        pop_operator(operators, &operatorsLen, output, &outputLen);
    
    Token retval = pop_token_stack(output, &outputLen);
    
    // TODO: cleanup, possibly??
    
    //let's not leak memory
    free(output);
    free(operators);
    
    return retval;
}

/*
 * Turns a token stream into an AST
 * Returns the AST in the argument tokens
 */
void create_AST(Token* tokens){
    if((*tokens)->type != BUILTIN){
        assert(0); // TODO: proper error handling
        // this should be a syntax error of some sort, probably
    }
    if(!can_start_line((*tokens)->builtin)){
        assert(0); // TODO: proper error handling
        // this should also be a syntax error
    }
    if(*tokens == NULL)
        return;
    
    // now we're assured of having a builtin, so let's grab it
    Token output = *tokens;
    *tokens = (*tokens)->next;
    
    // now let's grab the operands for our builtin
    
    // sanity checking
    if(get_arity(output->builtin) == 0 && *tokens != NULL){
        assert(0); // TODO: proper error handling
        // eg don't follow EXPAND with a value, etc
    } else if(get_arity(output->builtin) > get_token_length(*tokens)){
        assert(0); // TODO: proper error handling
        // first check for insufficient operands
    }
    
    // we only need to handle builtins with arity > 0 here
    // first operand goes in left
    // second operand goes in right
    // if three operands (ie bifurc)
    // make a dummy token as right
    // then second as its left and third as its right
    switch(output->builtin){
        // one name
        case INIT:
        case TERM:
        case PATH:
            if((*tokens)->type != NAME){
                assert(0); // TODO: proper error handling
            }
            output->left = *tokens;
            break;
        // one expression
        case OUTN:
        case OUTC:
            output->left = parse_infix_expression(*tokens);
            if(get_token_length(output->left) > 1){ // ie malformed expression
                assert(0); // TODO: proper error handling
            }
            break;
        // one name followed by one expression
        case SET:
            if((*tokens)->type != NAME){
                assert(0); // TODO: proper error handling
            }
            output->left = *tokens;
            output->right = parse_infix_expression((*tokens)->next);
            if(get_token_length(output->right) > 1){ // ie malformed expression
                assert(0); // TODO: proper error handling
            }
            break;
        // one expression followed by two names
        case BIFURC:
            // dummy token to hold the two paths
            output->right = make_builtin_token(BIFURC, NULL);
            // grab the two paths
            Token newEnd = *tokens;
            for(int i = 0; i < get_token_length(*tokens) - 3; i++)
                newEnd = newEnd->next;
            output->right->left = newEnd->next;
            output->right->right = newEnd->next->next;
            // we need to get names
            if(output->right->left->type != NAME || output->right->right->type != NAME){
                assert(0); // TODO: proper error handling
            }
            // chop the two paths we grabbed off the token stream
            newEnd->next = NULL;
            // grab the expression at last
            output->left = parse_infix_expression(*tokens);
            break;
        default: // do nothing for builtins with arity 0
            break;
    }
    
    // call to parse_infix_expression here as needed, based on arity, etc
    // possibly after grabbing non-expr operands, of course
    
    *tokens = output;
}