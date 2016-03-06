#include "parse.h"

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
void push_token_stack(Token t, Token* stack, unsigned* stackLength){
    t->next = *stack;
    (*stack) = t;
    (*stackLength)++;
}

/*
 * Pop a token off a stack
 */
Token pop_token_stack(Token* stack, unsigned* stackLength){
    if(*stackLength == 0)
        return NULL;
    Token retval = *stack;
    *stack = (*stack)->next;
    (*stackLength)--;
    return retval;
}

/*
 * Returns precedence for each operator
 * Higher values means higher precedence
 */
PrecedenceType get_precedence(BuiltinType b){
    switch(b){
        // the my-precedence-doesn't-real club:
        case EXPAND:
        case COLLAPSE:
        case INIT:
        case TERM:
        case SET:
        case PATH:
        case BIFURC:
        case OUTN:
        case OUTC:
        case IN:
        case L_PAREN:
        case R_PAREN:
            return 0;
        // actual precedence begins here
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
        case ADD:
        case SUBTRACT:
        case BIT_AND:
        case BIT_OR:
            return 4;
        case MULTIPLY:
        case DIVIDE:
            return 5;
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
        case IN:
        case NEG:
            return 1;
        case SET:
        case AND:
        case OR:
        case LESS_THAN:
        case GREATER_THAN:
        case L_SHIFT:
        case R_SHIFT:
        case ADD:
        case SUBTRACT:
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
        case IN:
            return 1;
        default:
            return 0;
    }
}

/*
 * Takes the top operator off of operators, grabs operands from output
 * and puts the new tree on output
 */
void pop_operator(Token* operators, unsigned* operatorsLen, Token* output, unsigned* outputLen){
    Token op = pop_token_stack(operators, operatorsLen);
    assert(op->type == BUILTIN);
    if(get_arity(op->builtin) == 1){
        if(*outputLen < 1){
            ERROR("insufficient operands (expected 1)");
        }
        op->left = pop_token_stack(output, outputLen);
    } else { // all the other infix ops are binary
        assert(get_arity(op->builtin) == 2);
        if(*outputLen < 2){
            ERROR("insufficient operands (expected 2)");
        }
        // stack, so fill in operands in reverse order
        op->right = pop_token_stack(output, outputLen);
        op->left = pop_token_stack(output, outputLen);
        assert(op->right != op->left); // guard against horrible bug from the before-times
    }
    push_token_stack(op, output, outputLen);
}


/*
 * Parse an infix expression from a token stream
 * Returns a parse tree
 * See https://en.wikipedia.org/wiki/Shunting-yard_algorithm
 */
Token parse_infix_expression(Token tokens){
    Token operators = NULL;
    unsigned operatorsLen = 0;
    
    Token output = NULL;
    unsigned outputLen = 0;
    
    Token nextToken;
    while(tokens != NULL){
        nextToken = tokens->next;
        tokens->next = NULL;
        // if it's not a builtin, stick it in output
        if(tokens->type != BUILTIN){
            push_token_stack(tokens, &output, &outputLen);
        } else { // if it's a builtin, do magic
            // check if it's allowed to be here
            ERROR_UNLESS(!can_start_line(tokens->builtin), "statement operators not allowed in expressions")
            if(operatorsLen == 0){
                push_token_stack(tokens, &operators, &operatorsLen);
            } else if(tokens->builtin == L_PAREN){
                push_token_stack(tokens, &operators, &operatorsLen);
            } else if(tokens->builtin == R_PAREN){
                // pop operators until L_PAREN
                free_token(&tokens); // we don't need the R_PAREN
                while(operatorsLen > 0 && operators->builtin != L_PAREN)
                    pop_operator(&operators, &operatorsLen, &output, &outputLen);
                if(operatorsLen == 0){ // there was no L_PAREN, so error
                    ERROR("mismatched parentheses")
                } else {
                    // discard the L_PAREN
                    Token discard = pop_token_stack(&operators, &operatorsLen);
                    free_token(&discard);
                }
            } else if(get_precedence(tokens->builtin) > get_precedence(operators->builtin)){
                push_token_stack(tokens, &operators, &operatorsLen);
            } else {
                // we are not a parenthesis and we're lower precedence than
                // the operator on the stack, so pop it and put us there
                pop_operator(&operators, &operatorsLen, &output, &outputLen);
                push_token_stack(tokens, &operators, &operatorsLen);
            }
        }
        tokens = nextToken;
    }
    
    // pop all remaining operators
    while(operatorsLen > 0)
        pop_operator(&operators, &operatorsLen, &output, &outputLen);
    
    ERROR_UNLESS(outputLen <= 1, "malformed expression") // leftover names or literals, so malformed expression
    
    Token retval = pop_token_stack(&output, &outputLen); // if stack was empty, retval <- NULL
    
    return retval;
}

/*
 * Turns a token stream into an AST
 * Returns the AST
 */
Token create_AST(Token tokens){
    if(tokens == NULL)
        return NULL;
    ERROR_UNLESS(tokens->type == BUILTIN, "line must start with operator")
    ERROR_UNLESS(can_start_line(tokens->builtin), "line must start with statement")
    
    // now we're assured of having an acceptable builtin, so let's grab it
    Token output = tokens;
    tokens = tokens->next;
    
    // now let's grab the operands for our builtin
    
    // sanity checking
    if(get_arity(output->builtin) == 0 && tokens != NULL){
        ERROR("too many operands (zero expected)")
        // eg don't follow EXPAND with a value, etc
    } else if(get_arity(output->builtin) > get_token_length(tokens)){
        ERROR("too few operands")
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
        case IN:
            ERROR_UNLESS(tokens->type == NAME, "name expected")
            ERROR_UNLESS(tokens->next == NULL, "too many operands (one expected)")
            output->left = tokens;
            break;
        // one expression
        case OUTN:
        case OUTC:
            output->left = parse_infix_expression(tokens);
            break;
        // one name followed by one expression
        case SET:
            ERROR_UNLESS(tokens->type == NAME, "name expected")
            output->left = tokens;
            output->right = parse_infix_expression(tokens->next);
            break;
        // one expression followed by two names
        case BIFURC:
            // dummy token to hold the two paths
            output->right = make_builtin_token(BIFURC, NULL);
            // grab the two paths
            Token newEnd = tokens;
            for(int i = 0; i < get_token_length(tokens) - 3; i++)
                newEnd = newEnd->next;
            output->right->left = newEnd->next;
            output->right->right = newEnd->next->next;
            // we need to get names
            if(output->right->left->type != NAME || output->right->right->type != NAME){
                ERROR("name expected")
            }
            // chop the two paths we grabbed off the token stream
            newEnd->next = NULL;
            // grab the expression at last
            output->left = parse_infix_expression(tokens);
            break;
        default: // do nothing for builtins with arity 0
            break;
    }
    
    return output;
}
