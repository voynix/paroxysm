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

/* make_path() */

void test_make_path(){
    TEST_GROUP_INDICATOR("make_path()")
    
    Path testPath = make_path(INITIAL_NAME, 1, NULL);
    
    assert(testPath != NULL);
    assert(testPath->name == INITIAL_NAME);
    assert(testPath->line == 1);
    assert(testPath->next == NULL);
}

/* free_path() */

void test_free_path(){
    TEST_GROUP_INDICATOR("free_path()")
    Path testPath = make_path(INITIAL_NAME + 2, 12, NULL);
    
    free_path(&testPath);
    
    assert(testPath == NULL);
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

/* create_variable() */

void test_create_variable_variable_exists(){
    Scope testScope = make_scope(NULL);
    testScope->variables = make_variable(INITIAL_NAME, 0, NULL);
    testScope->variables = make_variable(INITIAL_NAME + 1, 1, testScope->variables);
    testScope->variables = make_variable(INITIAL_NAME + 2, 2, testScope->variables);
    
    create_variable(testScope, INITIAL_NAME + 2, 12);
    
    assert(testScope->variables != NULL);
    assert(testScope->variables->name == INITIAL_NAME + 2);
    assert(testScope->variables->value == 2);
    assert(testScope->variables->next != NULL);
    assert(testScope->variables->next->name == INITIAL_NAME + 1);
    assert(testScope->variables->next->value == 1);
    assert(testScope->variables->next->next != NULL);
    assert(testScope->variables->next->next->name == INITIAL_NAME);
    assert(testScope->variables->next->next->value == 0);
    assert(testScope->variables->next->next->next == NULL);
}

void test_create_variable_variable_DNE(){
    Scope testScope = make_scope(NULL);
    testScope->variables = make_variable(INITIAL_NAME, 0, NULL);
    
    create_variable(testScope, INITIAL_NAME + 1, 1);
    
    assert(testScope->variables != NULL);
    assert(testScope->variables->name == INITIAL_NAME + 1);
    assert(testScope->variables->value == 1);
    assert(testScope->variables->next != NULL);
    assert(testScope->variables->next->name == INITIAL_NAME);
    assert(testScope->variables->next->value == 0);
    assert(testScope->variables->next->next == NULL);
}

void run_create_variable_tests(){
    TEST_GROUP_INDICATOR("create_variable()")
    
    test_create_variable_variable_exists();
    test_create_variable_variable_DNE();
}

/* set_variable() */

void test_set_variable(){
    TEST_GROUP_INDICATOR("set_variable()")
    
    Scope testScope = make_scope(NULL);
    testScope->variables = make_variable(INITIAL_NAME, 0, NULL);
    testScope->variables = make_variable(INITIAL_NAME + 1, 1, testScope->variables);
    testScope->variables = make_variable(INITIAL_NAME + 2, 2, testScope->variables);
    
    set_variable(testScope, INITIAL_NAME + 2, 12);
    
    assert(testScope->variables != NULL);
    assert(testScope->variables->name == INITIAL_NAME + 2);
    assert(testScope->variables->value == 12);
    assert(testScope->variables->next != NULL);
    assert(testScope->variables->next->name == INITIAL_NAME + 1);
    assert(testScope->variables->next->value == 1);
    assert(testScope->variables->next->next != NULL);
    assert(testScope->variables->next->next->name == INITIAL_NAME);
    assert(testScope->variables->next->next->value == 0);
    assert(testScope->variables->next->next->next == NULL);
}

/* get_variable() */

void test_get_variable(){
    TEST_GROUP_INDICATOR("get_variable()")
    
    Scope testScope = make_scope(NULL);
    testScope->variables = make_variable(INITIAL_NAME, 0, NULL);
    testScope->variables = make_variable(INITIAL_NAME + 1, 1, testScope->variables);
    testScope->variables = make_variable(INITIAL_NAME + 2, 2, testScope->variables);
    
    assert(get_variable(testScope, INITIAL_NAME + 2) == 2);
}

/* delete_variable() */

void test_delete_variable_one_var_one_scope(){
    Scope scope = make_scope(NULL);
    scope->variables = make_variable(INITIAL_NAME, 17, NULL);
    
    delete_variable(scope, INITIAL_NAME);
    
    assert(scope->variables == NULL);
}

void test_delete_variable_first_var_one_scope(){
    Scope scope = make_scope(NULL);
    scope->variables = make_variable(INITIAL_NAME, 12, NULL);
    Variable v = scope->variables;
    scope->variables = make_variable(INITIAL_NAME + 1, 11, scope->variables);
    
    delete_variable(scope, INITIAL_NAME + 1);
    
    assert(scope->variables == v);
    assert(scope->variables->next == NULL);
}

void test_delete_variable_middle_var_one_scope(){
    Scope scope = make_scope(NULL);
    scope->variables = make_variable(INITIAL_NAME, 12, NULL);
    Variable v = scope->variables;
    scope->variables = make_variable(INITIAL_NAME + 1, 11, scope->variables);
    scope->variables = make_variable(INITIAL_NAME + 2, 10, scope->variables);
    Variable w = scope->variables;
    
    delete_variable(scope, INITIAL_NAME + 1);
    
    assert(scope->variables == w);
    assert(scope->variables->next == v);
    assert(scope->variables->next->next == NULL);
}

void test_delete_variable_last_var_one_scope(){
    Scope scope = make_scope(NULL);
    scope->variables = make_variable(INITIAL_NAME, 12, NULL);
    scope->variables = make_variable(INITIAL_NAME + 1, 11, scope->variables);
    Variable v = scope->variables;
    
    delete_variable(scope, INITIAL_NAME);
    
    assert(scope->variables == v);
    assert(scope->variables->next == NULL);
}

void test_delete_variable_multiple_scopes(){
    // test to make sure that if multiple scopes point at the var
    // to be deleted they all get moved down
    Scope scopeA = make_scope(NULL);
    Scope scopeB = make_scope(scopeA);
    Scope scopeC = make_scope(scopeB);
    scopeA->variables = make_variable(INITIAL_NAME, 9, NULL);
    Variable v = scopeA->variables;
    scopeA->variables = make_variable(INITIAL_NAME + 1, -7, scopeA->variables);
    scopeB->variables = scopeA->variables;
    scopeC->variables = scopeB->variables;
    
    delete_variable(scopeC, INITIAL_NAME + 1);
    
    assert(scopeA->variables == v);
    assert(scopeB->variables == v);
    assert(scopeC->variables == v);
    assert(scopeA->variables->next == NULL);
}

void run_delete_variable_tests(){
    TEST_GROUP_INDICATOR("delete_variable()")
    
    test_delete_variable_one_var_one_scope();
    test_delete_variable_first_var_one_scope();
    test_delete_variable_middle_var_one_scope();
    test_delete_variable_last_var_one_scope();
    test_delete_variable_multiple_scopes();
}

/* get_path() */

void test_get_path(){
    TEST_GROUP_INDICATOR("get_path")
    Path pathList = make_path(INITIAL_NAME, 0, NULL);
    pathList = make_path(INITIAL_NAME + 1, 1, pathList);
    pathList = make_path(INITIAL_NAME + 2, 2, pathList);
    pathList = make_path(INITIAL_NAME + 3, 3, pathList);
    pathList = make_path(INITIAL_NAME + 4, 4, pathList);
    
    assert(get_path(pathList, INITIAL_NAME + 3) == 3);
}

/* push_scope_stack() */

void test_push_scope_stack_empty_scope_empty_stack(){
    Scope scope = make_scope(NULL);
    Scope stack = NULL;
    
    push_scope_stack(&stack, scope);
    
    assert(stack == scope);
    assert(stack->variables == NULL);
    assert(stack->next == NULL);
}

void test_push_scope_stack_empty_scope_stack(){
    Scope scopeA = make_scope(NULL);
    Scope scopeB = make_scope(NULL);
    Scope stack = scopeB;
    Variable v = make_variable(INITIAL_NAME, 12, NULL);
    stack->variables = v;
    
    push_scope_stack(&stack, scopeA);
    
    assert(stack == scopeA);
    assert(stack->variables == v);
    assert(stack->next == scopeB);
    assert(stack->next->variables == v);
    assert(stack->next->next == NULL);
}

void test_push_scope_stack_scope_empty_stack(){
    Scope scope = make_scope(NULL);
    Variable v = make_variable(INITIAL_NAME, -8, NULL);
    scope->variables = v;
    Variable w = make_variable(INITIAL_NAME + 1, 8, scope->variables);
    scope->variables = w;
    Scope stack = NULL;
    
    push_scope_stack(&stack, scope);
    
    assert(stack == scope);
    assert(stack->variables == w);
    assert(stack->variables->next == v);
    assert(stack->variables->next->next == NULL);
    assert(stack->next == NULL);
}

void test_push_scope_stack_scope_stack(){
    Scope scope = make_scope(NULL);
    Variable v = make_variable(INITIAL_NAME, -8, NULL);
    scope->variables = v;
    Variable w = make_variable(INITIAL_NAME + 1, 8, scope->variables);
    scope->variables = w;
    Scope stack = make_scope(NULL);
    Scope s = stack;
    Variable u = make_variable(INITIAL_NAME + 2, 0, NULL);
    stack->variables = u;

    push_scope_stack(&stack, scope);
    
    assert(stack == scope);
    assert(stack->variables == w);
    assert(stack->variables->next == v);
    assert(stack->variables->next->next == u);
    assert(stack->next == s);
    assert(stack->next->variables == u);
    assert(stack->next->variables->next == NULL);
    assert(stack->next->next == NULL);
}

void run_push_scope_stack_tests(){
    TEST_GROUP_INDICATOR("push_scope_stack()")
    
    test_push_scope_stack_empty_scope_empty_stack();
    test_push_scope_stack_empty_scope_stack();
    test_push_scope_stack_scope_empty_stack();
    test_push_scope_stack_scope_stack();
}

/* pop_scope_stack() */

void test_pop_scope_stack_empty_scope(){
    Scope scopeA = make_scope(NULL);
    Scope scopeB = make_scope(scopeA);
    Scope stack = scopeB;
    
    pop_scope_stack(&stack);
    
    assert(stack == scopeA);
    assert(scopeB->next == NULL);
}

void test_pop_scope_stack_scope_empty_stack(){
    Scope scope = make_scope(NULL);
    scope->variables = make_variable(INITIAL_NAME, 1, NULL);
    Scope stack = scope;
    
    pop_scope_stack(&stack);
    
    assert(stack == NULL);
    assert(scope->variables != NULL);
    assert(scope->variables->next == NULL);
    assert(scope->next == NULL);
}

void test_pop_scope_stack_scope_stack(){
    Scope scopeA = make_scope(NULL);
    Scope scopeB = make_scope(scopeA);
    scopeA->variables = make_variable(INITIAL_NAME, 0, NULL);
    scopeB->variables = make_variable(INITIAL_NAME + 1, 1, NULL);
    Scope stack = scopeA;
    push_scope_stack(&stack, scopeB);
    
    pop_scope_stack(&stack);
    
    assert(stack == scopeA);
    assert(scopeA->variables != NULL);
    assert(scopeA->variables->next == NULL);
    assert(scopeA->next == NULL);
    assert(scopeB->variables != NULL);
    assert(scopeB->variables->next == NULL);
    assert(scopeB->next == NULL);
}

void run_pop_scope_stack_tests(){
    TEST_GROUP_INDICATOR("pop_scope_stack()")
    
    test_pop_scope_stack_empty_scope();
    test_pop_scope_stack_scope_empty_stack();
    test_pop_scope_stack_scope_stack();
}

/* END TEST DECLARATIONS */

void run_evaluator_tests(){
    TEST_FILE_START_INDICATOR("evaluator")
    
    test_make_variable();
    test_free_variable();
    test_make_path();
    test_free_path();
    test_make_scope();
    test_free_scope();
    run_create_variable_tests();
    test_set_variable();
    test_get_variable();
    run_delete_variable_tests();
    test_get_path();
    run_push_scope_stack_tests();
    run_pop_scope_stack_tests();
    
    TEST_FILE_END_INDICATOR("evaluator")
}