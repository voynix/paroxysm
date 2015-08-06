#include "parse-test.h"

/* get_token_length */

void test_get_token_length_one_token(){
    Token t = make_builtin_token(COLLAPSE, NULL);
    
    assert(get_token_length(t) == 1);
}

void test_get_token_length_many_tokens(){
    Token t = make_builtin_token(L_PAREN, NULL);
    Token x = make_builtin_token(EXPAND, t);
    make_builtin_token(R_PAREN, x);
    
    assert(get_token_length(t) == 3);
}

void test_get_token_length_no_tokens(){
    assert(get_token_length(NULL) == 0);
}

void run_get_token_length_tests(){
    TEST_GROUP_INDICATOR("get_token_length()")
    test_get_token_length_one_token();
    test_get_token_length_many_tokens();
    test_get_token_length_no_tokens();
}

/* {push, pop}_token_stack() */

void test_push_token_stack(){
    Token (*test)[TOKEN_STACK_SIZE] = malloc(sizeof(test));
    unsigned len = 0;
    Token t = make_builtin_token(EXPAND, NULL);
    
    push_token_stack(t, test, &len);
    
    assert(len == 1);
    assert((*test)[0] == t);
}

void test_pop_token_stack_non_empty(){
    Token (*test)[TOKEN_STACK_SIZE] = malloc(sizeof(test));
    unsigned len = 0;
    Token t = make_builtin_token(EXPAND, NULL);
    push_token_stack(t, test, &len);
    
    Token x = pop_token_stack(test, &len);
    
    assert(len == 0);
    assert(x == t);
}

void test_pop_token_stack_empty(){
    Token (*test)[TOKEN_STACK_SIZE] = malloc(sizeof(test));
    unsigned len = 0;
    
    Token x = pop_token_stack(test, &len);
    
    assert(len == 0);
    assert(x == NULL);
}

void run_token_stack_tests(){
    TEST_GROUP_INDICATOR("..._token_stack()")
    test_push_token_stack();
    test_pop_token_stack_non_empty();
    test_pop_token_stack_empty();
}

/* get_precedence */

void test_get_precedence(){ // make sure we have values for all builtins
    TEST_GROUP_INDICATOR("get_precedence()")
    for(int i = 0; i < NUM_BUILTINS; i++)
        assert(get_precedence(i) != DEFAULT_PRECEDENCE);
}

/* get_arity */

void test_get_arity(){ // make sure we have values for all builtins
    TEST_GROUP_INDICATOR("get_arity()");
    for(int i = 0; i < NUM_BUILTINS; i++)
        assert(get_arity(i) != DEFAULT_ARITY);
}

/* can_start_line */

void test_can_start_line(){ // this is mostly pointless, but *shrug*
    TEST_GROUP_INDICATOR("can_start_line()");
    for(int i = 0; i < NUM_BUILTINS; i++)
        assert(can_start_line(i) == 1 || can_start_line(i) == 0);
}

/* pop_operator */

void test_pop_operator_unary_operators(){
    for(int i = 0; i < NUM_BUILTINS; i++){
        if(can_start_line(i))
            continue;
        if(get_arity(i) != 1)
            continue;
        Token (*operators)[TOKEN_STACK_SIZE] = malloc(sizeof(operators));
        Token (*output)[TOKEN_STACK_SIZE] = malloc(sizeof(output));
        unsigned operatorsLen = 0;
        unsigned outputLen = 0;
        push_token_stack(make_builtin_token(i, NULL), operators, &operatorsLen);
        push_token_stack(make_literal_token(2 << 30, NULL), output, &outputLen);
        
        pop_operator(operators, &operatorsLen, output, &outputLen);
        
        assert(operatorsLen == 0);
        assert(outputLen == 1);
        Token tokens = pop_token_stack(output, &outputLen);
        assert(tokens->type == BUILTIN);
        assert(tokens->builtin == i);
        assert(tokens->left != NULL);
        assert(tokens->right == NULL);
        assert(tokens->left->type == LITERAL);
        assert(tokens->left->literal == 2 << 30);
        assert(tokens->left->left == NULL);
        assert(tokens->left->right == NULL);
    }
}

void test_pop_operator_binary_operators(){
    for(int i = 0; i < NUM_BUILTINS; i++){
        if(can_start_line(i))
            continue;
        if(get_arity(i) != 2)
            continue;
        Token (*operators)[TOKEN_STACK_SIZE] = malloc(sizeof(operators));
        Token (*output)[TOKEN_STACK_SIZE] = malloc(sizeof(output));
        unsigned operatorsLen = 0;
        unsigned outputLen = 0;
        push_token_stack(make_builtin_token(i, NULL), operators, &operatorsLen);
        push_token_stack(make_literal_token(12, NULL), output, &outputLen);
        push_token_stack(make_literal_token(-13, NULL), output, &outputLen);
        
        pop_operator(operators, &operatorsLen, output, &outputLen);
        
        assert(operatorsLen == 0);
        assert(outputLen == 1);
        Token tokens = pop_token_stack(output, &outputLen);
        assert(tokens->type == BUILTIN);
        assert(tokens->builtin == i);
        assert(tokens->left != NULL);
        assert(tokens->right != NULL);
        assert(tokens->left->type == LITERAL);
        assert(tokens->left->literal == 12);
        assert(tokens->left->left == NULL);
        assert(tokens->left->right == NULL);
        assert(tokens->right->type == LITERAL);
        assert(tokens->right->literal == -13);
        assert(tokens->right->left == NULL);
        assert(tokens->right->right == NULL);
    }
}

void run_pop_operator_tests(){
    TEST_GROUP_INDICATOR("pop_operator()")
    test_pop_operator_unary_operators();
    test_pop_operator_binary_operators();
}

/* END TEST DECLARATIONS */

void run_parser_tests(){
    TEST_FILE_START_INDICATOR("parser")
    
    run_get_token_length_tests();
    run_token_stack_tests();
    test_get_precedence();
    test_get_arity();
    test_can_start_line();
    run_pop_operator_tests();
    
    TEST_FILE_END_INDICATOR("parser")
}