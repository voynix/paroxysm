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

/* END TEST DECLARATIONS */

void run_parser_tests(){
    TEST_FILE_START_INDICATOR("parser")
    
    run_token_array_tests();
    
    TEST_FILE_END_INDICATOR("parser")
}