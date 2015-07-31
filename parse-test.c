#include "parse-test.h"

/* {push, pop}_token_array() */

void test_push_token_array(){
    Token (*test)[TOKEN_ARRAY_SIZE] = malloc(sizeof(test));
    unsigned len = 0;
    Token t = make_builtin_token(EXPAND, NULL);
    
    push_token_array(t, test, &len);
    
    assert(len == 1);
    assert((*test)[0] == t);
}

void test_pop_token_array_non_empty(){
    Token (*test)[TOKEN_ARRAY_SIZE] = malloc(sizeof(test));
    unsigned len = 0;
    Token t = make_builtin_token(EXPAND, NULL);
    push_token_array(t, test, &len);
    
    Token x = pop_token_array(test, &len);
    
    assert(len == 0);
    assert(x == t);
}

void test_pop_token_array_empty(){
    Token (*test)[TOKEN_ARRAY_SIZE] = malloc(sizeof(test));
    unsigned len = 0;
    
    Token x = pop_token_array(test, &len);
    
    assert(len == 0);
    assert(x == NULL);
}

void run_token_array_tests(){
    TEST_GROUP_INDICATOR("..._token_array()")
    test_push_token_array();
    test_pop_token_array_non_empty();
    test_pop_token_array_empty();
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

/* END TEST DECLARATIONS */

void run_parser_tests(){
    TEST_FILE_START_INDICATOR("parser")
    
    run_token_array_tests();
    test_get_precedence();
    test_get_arity();
    test_can_start_line();
    
    TEST_FILE_END_INDICATOR("parser")
}