#include "test.h"

/* make_name() */

void test_make_name(){
    TEST_GROUP_INDICATOR("make_name()")
    char* testString = "test";
    
    Name testName = make_name(testString, 5, NULL);
    
    assert(strcmp(testName->fullName, testString) == 0);
    assert(testName->name == 5);
    assert(testName->next == NULL);
}

/* add_name() */

void test_add_name_null_namelist(){
    Name nameList = NULL;
    char* testString = "test";
    
    NameType testName = add_name(&nameList, testString);
    
    assert(testName == INITIAL_NAME);
    assert(nameList != NULL);
    assert(strcmp(nameList->fullName, testString) == 0);
    assert(nameList->name == INITIAL_NAME);
    assert(nameList->next == NULL);
}

void test_add_name_new_name(){
    char* foo = "foo";
    char* bar = "bar";
    Name nameList = make_name(foo, INITIAL_NAME, NULL);
    Name firstName = nameList;
    
    NameType testName = add_name(&nameList, bar);
    
    assert(testName == INITIAL_NAME + 1);
    assert(nameList != NULL);
    assert(strcmp(nameList->fullName, bar) == 0);
    assert(nameList->name == INITIAL_NAME + 1);
    assert(nameList->next == firstName);
}

void test_add_name_existing_name(){
    char* foo = "foo";
    char* bar = "bar";
    Name nameList = make_name(foo, INITIAL_NAME, NULL);
    nameList = make_name(bar, INITIAL_NAME + 1, nameList);
    
    assert(add_name(&nameList, foo) == INITIAL_NAME);
    assert(add_name(&nameList, bar) == INITIAL_NAME + 1);
}

void run_add_name_tests(){
    TEST_GROUP_INDICATOR("add_name()")
    test_add_name_null_namelist();
    test_add_name_new_name();
    test_add_name_existing_name();
}

/* make_{builtin, name, literal}_token */

void test_make_builtin_token(){
    Token testToken = make_builtin_token(EXPAND, NULL);
    
    assert(testToken->type == BUILTIN);
    assert(testToken->builtin == EXPAND);
    assert(testToken->next == NULL);
}

void test_make_name_token(){
    Token testToken = make_name_token(INITIAL_NAME, NULL);
    
    assert(testToken->type == NAME);
    assert(testToken->name == INITIAL_NAME);
    assert(testToken->next == NULL);
}

void test_make_literal_token(){
    LiteralType lit = 75;
    Token testToken = make_literal_token(lit, NULL);
    
    assert(testToken->type == LITERAL);
    assert(testToken->literal == lit);
    assert(testToken->next == NULL);
}

void run_make_token_tests(){
    TEST_GROUP_INDICATOR("make_..._token()")
    test_make_builtin_token();
    test_make_name_token();
    test_make_literal_token();
}

/* free_token() */

void test_free_token(){
    TEST_GROUP_INDICATOR("free_token()")
    Token t = make_builtin_token(DIVIDE, NULL);
    free_token(&t);
    assert(t == NULL);
}

/* get_token_end */

void test_get_token_end_first_token(){
    char* foo = "outn \t -12 hella";
    assert(get_token_end(foo, 0) == 4);
}

void test_get_token_end_middle_token(){
    char* foo = "outn \t-12  hella";
    assert(get_token_end(foo, 6) == 9);
}

void test_get_token_end_last_token(){
    char* foo = "outn -12  hella";
    assert(get_token_end(foo, 10) == 15);
}

void test_get_token_end_single_token(){
    char* foo = "expand";
    assert(get_token_end(foo, 0) == 6);
}

void test_get_token_end_bad_pos(){
    char* foo = "outn -12 hella";
    assert(get_token_end(foo, 20) == EOL);
}

void run_get_token_end_tests(){
    TEST_GROUP_INDICATOR("get_token_end()")
    test_get_token_end_first_token();
    test_get_token_end_middle_token();
    test_get_token_end_last_token();
    test_get_token_end_single_token();
    test_get_token_end_bad_pos();
}

/* get_token_start */

void test_get_token_start_first_token(){
    char* foo = "outn \t -12 hella";
    assert(get_token_start(foo, 0) == 0);
}

void test_get_token_start_middle_token(){
    char* foo = "outn \t-12  hella";
    assert(get_token_start(foo, 4) == 6);
}

void test_get_token_start_last_token(){
    char* foo = "outn -12  hella";
    assert(get_token_start(foo, 15) == EOL);
}

void test_get_token_start_single_token(){ // this is redundant with ...first_token, but eh
    char* foo = "expand";
    assert(get_token_start(foo, 0) == 0);
}

void test_get_token_start_bad_pos(){
    char* foo = "outn -12 hella";
    assert(get_token_start(foo, 20) == EOL);
}

void run_get_token_start_tests(){
    TEST_GROUP_INDICATOR("get_token_start()")
    test_get_token_start_first_token();
    test_get_token_start_middle_token();
    test_get_token_start_last_token();
    test_get_token_start_single_token();
    test_get_token_start_bad_pos();
}

/* END TEST DECLARATIONS */

int main(int argc, char** argv){
    test_make_name();
    run_add_name_tests();
    run_make_token_tests();
    test_free_token();
    run_get_token_end_tests();
    run_get_token_start_tests();
    
    printf("All tests passed!\n");
}