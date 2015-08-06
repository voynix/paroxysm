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
    Token (*test)[TOKEN_STACK_SIZE] = malloc(TOKEN_STACK_SIZE * sizeof(Token));
    unsigned len = 0;
    Token t = make_builtin_token(EXPAND, NULL);
    Token s = make_builtin_token(COLLAPSE, NULL);
    Token r = make_builtin_token(ADD, NULL);
    
    push_token_stack(t, test, &len);
    push_token_stack(s, test, &len);
    push_token_stack(r, test, &len);
    
    assert(len == 3);
    assert((*test)[0] == t);
    assert((*test)[1] == s);
    assert((*test)[2] == r);
}

void test_pop_token_stack_non_empty(){
    Token (*test)[TOKEN_STACK_SIZE] = malloc(TOKEN_STACK_SIZE * sizeof(Token));
    unsigned len = 0;
    Token t = make_builtin_token(EXPAND, NULL);
    Token s = make_builtin_token(EXPAND, NULL);
    push_token_stack(t, test, &len);
    push_token_stack(s, test, &len);
    
    Token x = pop_token_stack(test, &len);
    Token y = pop_token_stack(test, &len);
    
    assert(len == 0);
    assert(x == s);
    assert(y == t);
}

void test_pop_token_stack_empty(){
    Token (*test)[TOKEN_STACK_SIZE] = malloc(TOKEN_STACK_SIZE * sizeof(Token));
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
        Token (*operators)[TOKEN_STACK_SIZE] = malloc(TOKEN_STACK_SIZE * sizeof(Token));
        Token (*output)[TOKEN_STACK_SIZE] = malloc(TOKEN_STACK_SIZE * sizeof(Token));
        unsigned operatorsLen = 0;
        unsigned outputLen = 0;
        push_token_stack(make_builtin_token(i, NULL), operators, &operatorsLen);
        push_token_stack(make_literal_token(2 << 30, NULL), output, &outputLen);
        
        pop_operator(operators, &operatorsLen, output, &outputLen);
        
        assert(operatorsLen == 0);
        assert(outputLen == 1);
        Token tokens = pop_token_stack(output, &outputLen);
        assert(tokens != NULL);
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
        Token (*operators)[TOKEN_STACK_SIZE] = malloc(TOKEN_STACK_SIZE * sizeof(Token));
        Token (*output)[TOKEN_STACK_SIZE] = malloc(TOKEN_STACK_SIZE * sizeof(Token));
        unsigned operatorsLen = 0;
        unsigned outputLen = 0;
        push_token_stack(make_builtin_token(i, NULL), operators, &operatorsLen);
        push_token_stack(make_literal_token(12, NULL), output, &outputLen);
        push_token_stack(make_literal_token(-13, NULL), output, &outputLen);
        
        pop_operator(operators, &operatorsLen, output, &outputLen);
        
        assert(operatorsLen == 0);
        assert(outputLen == 1);
        Token tokens = pop_token_stack(output, &outputLen);
        assert(tokens != NULL);
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

/* parse_infix_expression */

void test_parse_infix_expression_unary_operators(){
    for(int i = 0; i < NUM_BUILTINS; i++){
        if(can_start_line(i))
            continue;
        if(get_arity(i) != 1)
            continue;
        Token t = make_builtin_token(i, NULL);
        make_literal_token(0, t);
        
        t = parse_infix_expression(t);
        
        assert(t != NULL);
        assert(t->type == BUILTIN);
        assert(t->builtin == i);
        assert(t->left != NULL);
        assert(t->right == NULL);
        assert(t->left->type == LITERAL);
        assert(t->left->literal == 0);
        assert(t->left->left == NULL);
        assert(t->left->right == NULL);
    }
}

void test_parse_infix_expression_binary_operators(){
    for(int i = 0; i < NUM_BUILTINS; i++){
        if(can_start_line(i))
            continue;
        if(get_arity(i) != 2)
            continue;
        Token t = make_literal_token(0, NULL);
        Token s = make_builtin_token(i, t);
        make_name_token(INITIAL_NAME, s);
        
        t = parse_infix_expression(t);
        
        assert(t != NULL);
        assert(t->type == BUILTIN);
        assert(t->builtin == i);
        assert(t->left != NULL);
        assert(t->right != NULL);
        assert(t->left->type == LITERAL);
        assert(t->left->literal == 0);
        assert(t->left->left == NULL);
        assert(t->left->right == NULL);
        assert(t->right->type == NAME);
        assert(t->right->name == INITIAL_NAME);
        assert(t->right->left == NULL);
        assert(t->right->right == NULL);
    }
}

void test_parse_infix_expression_parentheses(){
    Token t = make_literal_token(0, NULL);
    Token s = make_builtin_token(MULTIPLY, t);
    s = make_builtin_token(L_PAREN, s);
    s = make_name_token(INITIAL_NAME, s);
    s = make_builtin_token(ADD, s);
    s = make_name_token(INITIAL_NAME + 1, s);
    s = make_builtin_token(R_PAREN, s);
    
    t = parse_infix_expression(t);
    
    assert(t != NULL);
    assert(t->type == BUILTIN);
    assert(t->builtin == MULTIPLY);
    assert(t->left != NULL);
    assert(t->right != NULL);
    assert(t->left->type == LITERAL);
    assert(t->left->literal == 0);
    assert(t->left->left == NULL);
    assert(t->left->right == NULL);
    assert(t->right->type == BUILTIN);
    assert(t->right->builtin == ADD);
    assert(t->right->left != NULL);
    assert(t->right->right != NULL);
    assert(t->right->left->type == NAME);
    assert(t->right->left->name == INITIAL_NAME);
    assert(t->right->left->left == NULL);
    assert(t->right->left->right == NULL);
    assert(t->right->right->type == NAME);
    assert(t->right->right->name == INITIAL_NAME + 1);
    assert(t->right->right->left == NULL);
    assert(t->right->right->right == NULL);
}

void test_parse_infix_expression_precedence(){
    Token t = make_builtin_token(NEG, NULL);
    Token s = make_literal_token(-20, t);
    s = make_builtin_token(AND, s);
    s = make_literal_token(2, s);
    
    t = parse_infix_expression(t);
    
    assert(t != NULL);
    assert(t->type == BUILTIN);
    assert(t->builtin == AND);
    assert(t->left != NULL);
    assert(t->right != NULL);
    assert(t->left->type == BUILTIN);
    assert(t->left->builtin == NEG);
    assert(t->left->left != NULL);
    assert(t->left->right == NULL);
    assert(t->left->left->type == LITERAL);
    assert(t->left->left->literal == -20);
    assert(t->left->left->left == NULL);
    assert(t->left->left->right == NULL);
    assert(t->right->type == LITERAL);
    assert(t->right->literal == 2);
    assert(t->right->left == NULL);
    assert(t->right->right == NULL);
}

void test_parse_infix_expression_null_expression(){
    Token t = parse_infix_expression(NULL);
    
    assert(t == NULL);
}

void run_parse_infix_expression_tests(){
    TEST_GROUP_INDICATOR("parse_infix_expression()")
    test_parse_infix_expression_unary_operators();
    test_parse_infix_expression_binary_operators();
    test_parse_infix_expression_parentheses();
    test_parse_infix_expression_precedence();
    test_parse_infix_expression_null_expression();
}

/* create_AST */

void test_create_AST_INIT(){
    Token t = make_builtin_token(INIT, NULL);
    make_name_token(INITIAL_NAME, t);
    
    t = create_AST(t);
    
    assert(t != NULL);
    assert(t->type == BUILTIN);
    assert(t->builtin == INIT);
    assert(t->left != NULL);
    assert(t->right == NULL);
    assert(t->left->type == NAME);
    assert(t->left->name == INITIAL_NAME);
    assert(t->left->left == NULL);
    assert(t->left->right == NULL);
}

void test_create_AST_TERM(){
    Token t = make_builtin_token(TERM, NULL);
    make_name_token(INITIAL_NAME, t);
    
    t = create_AST(t);
    
    assert(t != NULL);
    assert(t->type == BUILTIN);
    assert(t->builtin == TERM);
    assert(t->left != NULL);
    assert(t->right == NULL);
    assert(t->left->type == NAME);
    assert(t->left->name == INITIAL_NAME);
    assert(t->left->left == NULL);
    assert(t->left->right == NULL);
}

void test_create_AST_PATH(){
    Token t = make_builtin_token(PATH, NULL);
    make_name_token(INITIAL_NAME, t);
    
    t = create_AST(t);
    
    assert(t != NULL);
    assert(t->type == BUILTIN);
    assert(t->builtin == PATH);
    assert(t->left != NULL);
    assert(t->right == NULL);
    assert(t->left->type == NAME);
    assert(t->left->name == INITIAL_NAME);
    assert(t->left->left == NULL);
    assert(t->left->right == NULL);
}

void test_create_AST_OUTN(){
    Token t = make_builtin_token(OUTN, NULL);
    make_literal_token(400, t);
    
    t = create_AST(t);
    
    assert(t != NULL);
    assert(t->type == BUILTIN);
    assert(t->builtin == OUTN);
    assert(t->left != NULL);
    assert(t->right == NULL);
    assert(t->left->type == LITERAL);
    assert(t->left->name == 400);
    assert(t->left->left == NULL);
    assert(t->left->right == NULL);
}

void test_create_AST_OUTC(){
    Token t = make_builtin_token(OUTC, NULL);
    Token s = make_builtin_token(NEG, t);
    make_literal_token(400, s);
    
    t = create_AST(t);
    
    assert(t != NULL);
    assert(t->type == BUILTIN);
    assert(t->builtin == OUTC);
    assert(t->left != NULL);
    assert(t->right == NULL);
    assert(t->left->type == BUILTIN);
    assert(t->left->builtin == NEG);
    assert(t->left->left != NULL);
    assert(t->left->right == NULL);
    assert(t->left->left->type == LITERAL);
    assert(t->left->left->name == 400);
    assert(t->left->left->left == NULL);
    assert(t->left->left->right == NULL);
}

void test_create_AST_SET(){
    Token t = make_builtin_token(SET, NULL);
    Token s = make_name_token(INITIAL_NAME, t);
    s = make_literal_token(10, s);
    s = make_builtin_token(OR, s);
    s = make_name_token(INITIAL_NAME + 1, s);
    
    t = create_AST(t);
    
    assert(t != NULL);
    assert(t->type == BUILTIN);
    assert(t->builtin == SET);
    assert(t->left != NULL);
    assert(t->right != NULL);
    assert(t->left->type == NAME);
    assert(t->left->builtin == INITIAL_NAME);
    assert(t->left->left == NULL);
    assert(t->left->right == NULL);
    assert(t->right->type == BUILTIN);
    assert(t->right->builtin == OR);
    assert(t->right->left != NULL);
    assert(t->right->right != NULL);
    assert(t->right->left->type == LITERAL);
    assert(t->right->left->literal == 10);
    assert(t->right->left->left == NULL);
    assert(t->right->left->right == NULL);
    assert(t->right->right->type == NAME);
    assert(t->right->right->name == INITIAL_NAME + 1);
    assert(t->right->right->left == NULL);
    assert(t->right->right->right == NULL);
}

void test_create_AST_BIFURC(){
    Token t = make_builtin_token(BIFURC, NULL);
    Token s = make_builtin_token(NEG, t);
    s = make_builtin_token(L_PAREN, s);
    s = make_builtin_token(L_PAREN, s);
    s = make_builtin_token(NEG, s);
    s = make_literal_token(14, s);
    s = make_builtin_token(R_PAREN, s);
    s = make_builtin_token(R_PAREN, s);
    s = make_name_token(INITIAL_NAME, s);
    s = make_name_token(INITIAL_NAME + 1, s);
    
    t = create_AST(t);
    
    assert(t != NULL);
    assert(t->type == BUILTIN);
    assert(t->builtin == BIFURC);
    assert(t->left != NULL);
    assert(t->right != NULL);
    assert(t->left->type == BUILTIN);
    assert(t->left->builtin == NEG);
    assert(t->left->left != NULL);
    assert(t->left->right == NULL);
    assert(t->left->left->type == BUILTIN);
    assert(t->left->left->builtin == NEG);
    assert(t->left->left->left != NULL);
    assert(t->left->left->right == NULL);
    assert(t->left->left->left->type == LITERAL);
    assert(t->left->left->left->literal == 14);
    assert(t->left->left->left->left == NULL);
    assert(t->left->left->left->right == NULL);
    assert(t->right->type == BUILTIN);
    assert(t->right->builtin == BIFURC);
    assert(t->right->left != NULL);
    assert(t->right->right != NULL);
    assert(t->right->left->type == NAME);
    assert(t->right->left->name == INITIAL_NAME);
    assert(t->right->left->left == NULL);
    assert(t->right->left->right == NULL);
    assert(t->right->right->type == NAME);
    assert(t->right->right->name == INITIAL_NAME + 1);
    assert(t->right->right->left == NULL);
    assert(t->right->right->right == NULL);
}

void test_create_AST_null_stream(){
    Token t = create_AST(NULL);
    
    assert(t == NULL);
}

void run_create_AST_tests(){
    TEST_GROUP_INDICATOR("create_AST()")
    test_create_AST_INIT();
    test_create_AST_TERM();
    test_create_AST_PATH();
    test_create_AST_OUTN();
    test_create_AST_OUTC();
    test_create_AST_SET();
    test_create_AST_BIFURC();
    test_create_AST_null_stream();
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
    run_parse_infix_expression_tests();
    run_create_AST_tests();
    
    TEST_FILE_END_INDICATOR("parser")
}