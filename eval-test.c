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

/* make_sscope() */

void test_make_sscope(){
    TEST_GROUP_INDICATOR("make_sscope()")
    Scope scope = make_scope(NULL);
    
    SScope testSScope = make_sscope(1, scope, NULL);
    
    assert(testSScope != NULL);
    assert(testSScope->line == 1);
    assert(testSScope->scope == scope);
    assert(testSScope->next == NULL);
}

/* free_sscope() */

void test_free_sscope(){
    TEST_GROUP_INDICATOR("free_sscope()")
    Scope scope = make_scope(NULL);
    SScope testSScope = make_sscope(1, scope, NULL);
    
    free_sscope(&testSScope);
    
    assert(testSScope == NULL);
    // we can't really confirm that the inner scope got deleted
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
    TEST_GROUP_INDICATOR("get_path()")
    Path pathList = make_path(INITIAL_NAME, 0, NULL);
    pathList = make_path(INITIAL_NAME + 1, 1, pathList);
    pathList = make_path(INITIAL_NAME + 2, 2, pathList);
    pathList = make_path(INITIAL_NAME + 3, 3, pathList);
    pathList = make_path(INITIAL_NAME + 4, 4, pathList);
    
    assert(get_path(pathList, INITIAL_NAME + 3) == 3);
}

/* get_scope() */

void test_get_scope(){
    TEST_GROUP_INDICATOR("get_scope()")
    Scope scope = make_scope(NULL);
    SScope scopeList = make_sscope(0, make_scope(NULL), NULL);
    scopeList = make_sscope(1, scope, scopeList);
    scopeList = make_sscope(2, make_scope(NULL), scopeList);
    scopeList = make_sscope(3, make_scope(NULL), scopeList);
    scopeList = make_sscope(4, make_scope(NULL), scopeList);
    
    assert(get_scope(scopeList, 1) == scope);
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

/* preevaluate_AST() */

void test_preevaluate_AST_path(){
    Path pathList = NULL;
    SScope scopeList = NULL;
    Token tokens = make_builtin_token(PATH, NULL);
    tokens->left = make_name_token(INITIAL_NAME, NULL);
    
    preevaluate_AST(tokens, 12, &pathList, &scopeList);
    
    assert(pathList != NULL);
    assert(pathList->name == INITIAL_NAME);
    assert(pathList->line == 12);
    assert(pathList->next == NULL);
    assert(scopeList == NULL);
}

void test_preevaluate_AST_scope(){
    Path pathList = NULL;
    SScope scopeList = NULL;
    Token tokens = make_builtin_token(EXPAND, NULL);
    
    preevaluate_AST(tokens, 12, &pathList, &scopeList);
    
    assert(pathList == NULL);
    assert(scopeList != NULL);
    assert(scopeList->line == 12);
    assert(scopeList->scope != NULL);
    assert(scopeList->next == NULL);
}

void run_preevaluate_AST_tests(){
    TEST_GROUP_INDICATOR("preevaluate_AST()")
    
    test_preevaluate_AST_path();
    test_preevaluate_AST_scope();
}

/* evaluate_AST() */

void test_evaluate_AST_literal(){
    Token tokens = make_literal_token(-17, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    
    assert(evaluate_AST(tokens, line, NULL, NULL, NULL, &nextLine) == -17);
    
    assert(nextLine == 11);
}

void test_evaluate_AST_name(){
    Token tokens = make_name_token(INITIAL_NAME, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    Scope scopeStack = make_scope(NULL);
    scopeStack->variables = make_variable(INITIAL_NAME, -17, NULL);
    
    assert(evaluate_AST(tokens, line, &scopeStack, NULL, NULL, &nextLine) == -17);
    
    assert(nextLine == 11);
}

void test_evaluate_AST_expand(){
    Token tokens = make_builtin_token(EXPAND, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    Scope scopeStack = NULL;
    SScope scopeList = make_sscope(10, make_scope(NULL), NULL);
    
    assert(evaluate_AST(tokens, line, &scopeStack, NULL, scopeList, &nextLine) == DEFAULT_AST_VALUE);
    
    assert(scopeStack == scopeList->scope);
    assert(nextLine == 11);
}

void test_evaluate_AST_collapse(){
    Token tokens = make_builtin_token(COLLAPSE, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    Scope scopeStack = make_scope(NULL);
    
    assert(evaluate_AST(tokens, line, &scopeStack, NULL, NULL, &nextLine) == DEFAULT_AST_VALUE);
    
    assert(scopeStack == NULL);
    assert(nextLine == 11);
}

void test_evaluate_AST_init(){
    Token tokens = make_builtin_token(INIT, NULL);
    tokens->left = make_name_token(INITIAL_NAME, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    Scope scopeStack = make_scope(NULL);
    
    assert(evaluate_AST(tokens, line, &scopeStack, NULL, NULL, &nextLine) == DEFAULT_AST_VALUE);
    
    assert(scopeStack->variables != NULL);
    assert(scopeStack->variables->name == INITIAL_NAME);
    assert(nextLine == 11);
}

void test_evaluate_AST_term(){
    Token tokens = make_builtin_token(TERM, NULL);
    tokens->left = make_name_token(INITIAL_NAME, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    Scope scopeStack = make_scope(NULL);
    scopeStack->variables = make_variable(INITIAL_NAME, 12, NULL);
    
    assert(evaluate_AST(tokens, line, &scopeStack, NULL, NULL, &nextLine) == DEFAULT_AST_VALUE);
    
    assert(scopeStack->variables == NULL);
    assert(nextLine == 11);
}

void test_evaluate_AST_set(){
    Token tokens = make_builtin_token(SET, NULL);
    tokens->left = make_name_token(INITIAL_NAME, NULL);
    tokens->right = make_literal_token(270000, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    Scope scopeStack = make_scope(NULL);
    scopeStack->variables = make_variable(INITIAL_NAME, 12, NULL);
    
    assert(evaluate_AST(tokens, line, &scopeStack, NULL, NULL, &nextLine) == DEFAULT_AST_VALUE);
    
    assert(scopeStack->variables != NULL);
    assert(scopeStack->variables->name == INITIAL_NAME);
    assert(scopeStack->variables->value == 270000);
    assert(nextLine == 11);
}

void test_evaluate_AST_bifurc_true(){
    Token tokens = make_builtin_token(BIFURC, NULL);
    tokens->left = make_literal_token(14, NULL);
    tokens->right = make_builtin_token(BIFURC, NULL);
    tokens->right->left = make_name_token(INITIAL_NAME, NULL);
    tokens->right->right = make_name_token(INITIAL_NAME + 1, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    Path pathList = make_path(INITIAL_NAME, 14, make_path(INITIAL_NAME + 1, 16, NULL));
    
    assert(evaluate_AST(tokens, line, NULL, pathList, NULL, &nextLine) == DEFAULT_AST_VALUE);
    
    assert(nextLine == 14);
}

void test_evaluate_AST_bifurc_false(){
    Token tokens = make_builtin_token(BIFURC, NULL);
    tokens->left = make_literal_token(0, NULL);
    tokens->right = make_builtin_token(BIFURC, NULL);
    tokens->right->left = make_name_token(INITIAL_NAME, NULL);
    tokens->right->right = make_name_token(INITIAL_NAME + 1, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    Path pathList = make_path(INITIAL_NAME, 14, make_path(INITIAL_NAME + 1, 16, NULL));
    
    assert(evaluate_AST(tokens, line, NULL, pathList, NULL, &nextLine) == DEFAULT_AST_VALUE);
    
    assert(nextLine == 16);
}

void test_evaluate_AST_outn(){
    // TODO: work out how to test STDOUT
}

void test_evaluate_AST_outc(){
    // TODO: work out how to test STDOUT
}

void test_evaluate_AST_and_both_true(){
    Token tokens = make_builtin_token(AND, NULL);
    tokens->left = make_literal_token(14, NULL);
    tokens->right = make_literal_token(11, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    
    assert(evaluate_AST(tokens, line, NULL, NULL, NULL, &nextLine));
    
    assert(nextLine == 11);
}

void test_evaluate_AST_and_first_true(){
    Token tokens = make_builtin_token(AND, NULL);
    tokens->left = make_literal_token(14, NULL);
    tokens->right = make_literal_token(0, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    
    assert(!evaluate_AST(tokens, line, NULL, NULL, NULL, &nextLine));
    
    assert(nextLine == 11);
}

void test_evaluate_AST_and_second_true(){
    Token tokens = make_builtin_token(AND, NULL);
    tokens->left = make_literal_token(0, NULL);
    tokens->right = make_literal_token(11, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    
    assert(!evaluate_AST(tokens, line, NULL, NULL, NULL, &nextLine));
    
    assert(nextLine == 11);
}

void test_evaluate_AST_and_neither_true(){
    Token tokens = make_builtin_token(AND, NULL);
    tokens->left = make_literal_token(0, NULL);
    tokens->right = make_literal_token(0, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    
    assert(!evaluate_AST(tokens, line, NULL, NULL, NULL, &nextLine));
    
    assert(nextLine == 11);
}

void test_evaluate_AST_or_both_true(){
    Token tokens = make_builtin_token(OR, NULL);
    tokens->left = make_literal_token(14, NULL);
    tokens->right = make_literal_token(11, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    
    assert(evaluate_AST(tokens, line, NULL, NULL, NULL, &nextLine));
    
    assert(nextLine == 11);
}

void test_evaluate_AST_or_first_true(){
    Token tokens = make_builtin_token(OR, NULL);
    tokens->left = make_literal_token(14, NULL);
    tokens->right = make_literal_token(0, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    
    assert(evaluate_AST(tokens, line, NULL, NULL, NULL, &nextLine));
    
    assert(nextLine == 11);
}

void test_evaluate_AST_or_second_true(){
    Token tokens = make_builtin_token(OR, NULL);
    tokens->left = make_literal_token(0, NULL);
    tokens->right = make_literal_token(11, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    
    assert(evaluate_AST(tokens, line, NULL, NULL, NULL, &nextLine));
    
    assert(nextLine == 11);
}

void test_evaluate_AST_or_neither_true(){
    Token tokens = make_builtin_token(OR, NULL);
    tokens->left = make_literal_token(0, NULL);
    tokens->right = make_literal_token(0, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    
    assert(!evaluate_AST(tokens, line, NULL, NULL, NULL, &nextLine));
    
    assert(nextLine == 11);
}

void test_evaluate_AST_add(){
    Token tokens = make_builtin_token(ADD, NULL);
    tokens->left = make_literal_token(-10, NULL);
    tokens->right = make_literal_token(400, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    
    assert(evaluate_AST(tokens, line, NULL, NULL, NULL, &nextLine) == 390);
    
    assert(nextLine == 11);
}

void test_evaluate_AST_subtract(){
    Token tokens = make_builtin_token(SUBTRACT, NULL);
    tokens->left = make_literal_token(70000, NULL);
    tokens->right = make_literal_token(400, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    
    assert(evaluate_AST(tokens, line, NULL, NULL, NULL, &nextLine) == 69600);
    
    assert(nextLine == 11);
}

void test_evaluate_AST_multiply(){
    Token tokens = make_builtin_token(MULTIPLY, NULL);
    tokens->left = make_literal_token(400, NULL);
    tokens->right = make_literal_token(-2, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    
    assert(evaluate_AST(tokens, line, NULL, NULL, NULL, &nextLine) == -800);
    
    assert(nextLine == 11);
}

void test_evaluate_AST_divide(){
    Token tokens = make_builtin_token(DIVIDE, NULL);
    tokens->left = make_literal_token(27, NULL);
    tokens->right = make_literal_token(9, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    
    assert(evaluate_AST(tokens, line, NULL, NULL, NULL, &nextLine) == 3);
    
    assert(nextLine == 11);
}

void test_evaluate_AST_l_shift(){
    Token tokens = make_builtin_token(L_SHIFT, NULL);
    tokens->left = make_literal_token(10, NULL);
    tokens->right = make_literal_token(4, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    
    assert(evaluate_AST(tokens, line, NULL, NULL, NULL, &nextLine) == 10 << 4);
    
    assert(nextLine == 11);
}

void test_evaluate_AST_r_shift(){
    Token tokens = make_builtin_token(R_SHIFT, NULL);
    tokens->left = make_literal_token(1000, NULL);
    tokens->right = make_literal_token(4, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    
    assert(evaluate_AST(tokens, line, NULL, NULL, NULL, &nextLine) == 1000 >> 4);
    
    assert(nextLine == 11);
}

void test_evaluate_AST_less_than_true(){
    Token tokens = make_builtin_token(LESS_THAN, NULL);
    tokens->left = make_literal_token(-20, NULL);
    tokens->right = make_literal_token(34567, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    
    assert(evaluate_AST(tokens, line, NULL, NULL, NULL, &nextLine));
    
    assert(nextLine == 11);
}

void test_evaluate_AST_less_than_false(){
    Token tokens = make_builtin_token(LESS_THAN, NULL);
    tokens->left = make_literal_token(2000000, NULL);
    tokens->right = make_literal_token(34567, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    
    assert(!evaluate_AST(tokens, line, NULL, NULL, NULL, &nextLine));
    
    assert(nextLine == 11);
}

void test_evaluate_AST_greater_than_true(){
    Token tokens = make_builtin_token(GREATER_THAN, NULL);
    tokens->left = make_literal_token(2, NULL);
    tokens->right = make_literal_token(-1, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    
    assert(evaluate_AST(tokens, line, NULL, NULL, NULL, &nextLine));
    
    assert(nextLine == 11);
}

void test_evaluate_AST_greater_than_false(){
    Token tokens = make_builtin_token(GREATER_THAN, NULL);
    tokens->left = make_literal_token(20000, NULL);
    tokens->right = make_literal_token(34567, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    
    assert(!evaluate_AST(tokens, line, NULL, NULL, NULL, &nextLine));
    
    assert(nextLine == 11);
}

void test_evaluate_AST_bit_and(){
    Token tokens = make_builtin_token(BIT_AND, NULL);
    tokens->left = make_literal_token(10, NULL);
    tokens->right = make_literal_token(6, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    
    assert(evaluate_AST(tokens, line, NULL, NULL, NULL, &nextLine) == 2);
    
    assert(nextLine == 11);
}

void test_evaluate_AST_bit_or(){
    Token tokens = make_builtin_token(BIT_OR, NULL);
    tokens->left = make_literal_token(10, NULL);
    tokens->right = make_literal_token(6, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    
    assert(evaluate_AST(tokens, line, NULL, NULL, NULL, &nextLine) == 14);
    
    assert(nextLine == 11);
}

void test_evaluate_AST_neg(){
    Token tokens = make_builtin_token(NEG, NULL);
    tokens->left = make_literal_token(10, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    
    assert(evaluate_AST(tokens, line, NULL, NULL, NULL, &nextLine) == ~ 10);
    
    assert(nextLine == 11);
}

void test_evaluate_AST_defaults(){
    Token tokens = make_builtin_token(PATH, NULL);
    tokens->left = make_name_token(INITIAL_NAME, NULL);
    LineType line = 10;
    LineType nextLine = 11;
    
    assert(evaluate_AST(tokens, line, NULL, NULL, NULL, &nextLine) == DEFAULT_AST_VALUE);
    
    assert(nextLine == 11);
    
    tokens = make_builtin_token(L_PAREN, NULL);
    
    assert(evaluate_AST(tokens, line, NULL, NULL, NULL, &nextLine) == DEFAULT_AST_VALUE);
    
    assert(nextLine == 11);
    
    tokens = make_builtin_token(R_PAREN, NULL);
    
    assert(evaluate_AST(tokens, line, NULL, NULL, NULL, &nextLine) == DEFAULT_AST_VALUE);
    
    assert(nextLine == 11);
}

void run_evaluate_AST_tests(){
    TEST_GROUP_INDICATOR("evaluate_AST()")
    
    test_evaluate_AST_literal();
    test_evaluate_AST_name();
    test_evaluate_AST_expand();
    test_evaluate_AST_collapse();
    test_evaluate_AST_init();
    test_evaluate_AST_term();
    test_evaluate_AST_set();
    test_evaluate_AST_bifurc_true();
    test_evaluate_AST_bifurc_false();
    test_evaluate_AST_outn();
    test_evaluate_AST_outc();
    test_evaluate_AST_and_both_true();
    test_evaluate_AST_and_first_true();
    test_evaluate_AST_and_second_true();
    test_evaluate_AST_and_neither_true();
    test_evaluate_AST_or_both_true();
    test_evaluate_AST_or_first_true();
    test_evaluate_AST_or_second_true();
    test_evaluate_AST_or_neither_true();
    test_evaluate_AST_add();
    test_evaluate_AST_subtract();
    test_evaluate_AST_multiply();
    test_evaluate_AST_divide();
    test_evaluate_AST_l_shift();
    test_evaluate_AST_r_shift();
    test_evaluate_AST_less_than_true();
    test_evaluate_AST_less_than_false();
    test_evaluate_AST_greater_than_true();
    test_evaluate_AST_greater_than_false();
    test_evaluate_AST_bit_and();
    test_evaluate_AST_bit_or();
    test_evaluate_AST_neg();
    test_evaluate_AST_defaults();
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
    test_make_sscope();
    test_free_sscope();
    run_create_variable_tests();
    test_set_variable();
    test_get_variable();
    run_delete_variable_tests();
    test_get_path();
    test_get_scope();
    run_push_scope_stack_tests();
    run_pop_scope_stack_tests();
    run_preevaluate_AST_tests();
    run_evaluate_AST_tests();
    
    TEST_FILE_END_INDICATOR("evaluator")
}