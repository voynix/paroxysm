#include "eval-test.h"

/* make_variable() */

void test_make_variable(){
    TEST_GROUP_INDICATOR("make_variable()")
    
    Variable testVar = make_variable(INITIAL_NAME + 4, 17, NULL);
    
    assert(testVar != NULL);
    assert(testVar->name == INITIAL_NAME + 4);
    assert(testVar->value == 17);
    assert(testVar->next == NULL);
}

/* free_variable() */

void test_free_variable(){
    TEST_GROUP_INDICATOR("free_variable()")
    Variable testVar = make_variable(INITIAL_NAME + 2, 12, NULL);
    
    free_variable(&testVar);
    
    assert(testVar == NULL);
}

/* make_scope() */

void test_make_scope(){
    TEST_GROUP_INDICATOR("make_scope()")
    
    Scope testScope = make_scope(NULL);
    
    assert(testScope != NULL);
    assert(testScope->variables == NULL);
    assert(testScope->next == NULL);
}

/* free_scope() */

void test_free_scope(){
    TEST_GROUP_INDICATOR("free_scope()")
    Scope testScope = make_scope(NULL);
    testScope->variables = make_variable(INITIAL_NAME, 0, NULL);
    testScope->variables = make_variable(INITIAL_NAME + 1, 1, testScope->variables);
    testScope->variables = make_variable(INITIAL_NAME + 2, 2, testScope->variables);
    testScope->variables = make_variable(INITIAL_NAME + 3, 3, testScope->variables);
    
    free_scope(&testScope);
    
    assert(testScope == NULL);
    // we can't really confirm that all the variables were deleted
    // but this is the best we can do, so *shrug*
}

/* END TEST DECLARATIONS */

void run_evaluator_tests(){
    TEST_FILE_START_INDICATOR("evaluator")
    
    test_make_variable();
    test_free_variable();
    test_make_scope();
    test_free_scope();
    
    TEST_FILE_END_INDICATOR("evaluator")
}