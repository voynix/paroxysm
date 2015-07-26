#include "test.h"

/* make_name() */

void test_make_name(){
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
    test_make_builtin_token();
    test_make_name_token();
    test_make_literal_token();
}

/* END TEST DECLARATIONS */

int main(int argc, char** argv){
    printf("%s", "Testing make_name()...\n");
    test_make_name();
    
    printf("%s", "Testing add_name()...\n");
    run_add_name_tests();
    
    printf("%s", "Testing make_..._token()...\n");
    run_make_token_tests();
    
    printf("%s", "All tests passed!\n");
}