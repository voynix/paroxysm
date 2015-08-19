#include "eval.h"

const ValueType DEFAULT_AST_VALUE = 23; // !SIRE LIAH
const ValueType DEFAULT_VARIABLE_VALUE = 0;
const ValueType MAX_PRINTABLE_CHAR = 128; // 7-bit ASCII only

/*
 * Creates a new VariableRec with the given values
 * Returns the created VariableRec
 */
Variable make_variable(NameType name, ValueType value, Variable next){
    Variable retVal = (Variable) malloc(sizeof(VariableRec));
    assert(retVal != NULL);
    retVal->name = name;
    retVal->value = value;
    retVal->next = next;
    return retVal;
}

/*
 * Frees a Variable
 */
void free_variable(Variable* variable){
    assert(*variable != NULL);
    free(*variable);
    *variable = NULL;
}

/*
 * Creates a new PathRec with the given values
 * Returns the created PathRec
 */
Path make_path(NameType name, LineType line, Path next){
    Path retVal = (Path) malloc(sizeof(PathRec));
    assert(retVal != NULL);
    retVal->name = name;
    retVal->line = line;
    retVal->next = next;
    return retVal;
}

/*
 * Frees a Path
 */
void free_path(Path* path){
    assert(*path != NULL);
    free(*path);
    *path = NULL;
}

/*
 * Creates a new ScopeRec with the given values
 * Returns the created ScopeRec
 */
Scope make_scope(Scope next){
    Scope retVal = (Scope) malloc(sizeof(ScopeRec));
    assert(retVal != NULL);
    retVal->variables = NULL;
    retVal->next = next;
    return retVal;
}

/*
 * Frees a Scope, including all of its VariableRecs
 * Note that (*scope)->variables MUST be NULL-terminated
 * or free_scope will run off the end of the list
 */
void free_scope(Scope* scope){
    assert(*scope != NULL);
    Variable nextVar;
    while((*scope)->variables != NULL){
        nextVar = (*scope)->variables->next;
        free((*scope)->variables);
        (*scope)->variables = nextVar;
    }
    free(*scope);
    *scope = NULL;
}

/*
 * Creates a new ScopeRecRec with the given values
 * Returns the created ScopeRecRec
 */
SScope make_sscope(LineType line, Scope scope, SScope next){
    SScope retVal = (SScope) malloc(sizeof(ScopeRecRec));
    assert(retVal != NULL);
    retVal->line = line;
    retVal->scope = scope;
    retVal->next = next;
    return retVal;
}

/*
 * Frees a SScope and its internal Scope
 */
void free_sscope(SScope* sscope){
    assert(*sscope != NULL);
    free_scope(&(*sscope)->scope);
    free(*sscope);
    *sscope = NULL;
}

/*
 * Create a new Variable with the given values in the current Scope 
 * if no Variable by the given name exists already
 */
void create_variable(Scope scope, NameType name, ValueType value){
    assert(scope != NULL);
    
    Variable v = scope->variables;
    while(v != NULL){
        // if a variable with this name already exists somewhere, we're done
        if(name == v->name)
            return;
        v = v->next;
    }
    scope->variables = make_variable(name, value, scope->variables);
}

/*
 * Set the given Variable to the given value, assuming it exists
 */
void set_variable(Scope scope, NameType name, ValueType value){
    assert(scope != NULL);
    
    Variable v = scope->variables;
    while(v != NULL){
        if(name == v->name){
            v->value = value;
            return;
        }
        v = v->next;
    }
    assert(0); // TODO: proper error handling
}

/*
 * Get the value for the given variable, assuming it exists
 */
ValueType get_variable(Scope scope, NameType name){
    assert(scope != NULL);
    
    Variable v = scope->variables;
    while(v != NULL){
        if(name == v->name)
            return v->value;
        v = v->next;
    }
    assert(0); // TODO: proper error handling
}

/*
 * Remove the first instance of the given variable found
 */
void delete_variable(Scope scope, NameType name){
    assert(scope != NULL);
    
    Variable currentVariable = scope->variables;
    Variable prevVariable = NULL;
    
    while(currentVariable != NULL){
        // if we've found the variable, delete it
        if(name == currentVariable->name){
            if(prevVariable != NULL)
                prevVariable->next = currentVariable->next;
            // if we have multiple scopes pointing at this variable
            // walk the entire scope list, correcting as we go
            for(Scope s = scope; s != NULL; s = s->next)
                if(s->variables == currentVariable)
                    s->variables = currentVariable->next;
            free_variable(&currentVariable);
            return;
        }
        
        // let's walk
        prevVariable = currentVariable;
        currentVariable = currentVariable->next;
    }
    
    // we didn't find the variable to delete, so something went wrong
    assert(0); // TODO: proper error handling
}

/*
 * Returns the line number for the given path
 */
LineType get_path(Path pathList, NameType name){
    assert(pathList != NULL);
    
    while(pathList != NULL){
        if(pathList->name == name)
            return pathList->line;
        pathList = pathList->next;
    }

    // we didn't find the path, so something went wrong
    assert(0); // TODO: proper error handling
}

/*
 * Returns the scope for a given line
 */
Scope get_scope(SScope scopeList, LineType line){
    assert(scopeList != NULL);
    
    while(scopeList != NULL){
        if(scopeList->line == line)
            return scopeList->scope;
        scopeList = scopeList->next;
    }
    
    // we didn't find the scope, so something went horribly wrong
    assert(0); // TODO: proper error handling; this shouldn't happen
}

/*
 * Push the given scope onto the stack of active scopes
 * If the given scope is already on the stack, do nothing
 */
void push_scope_stack(Scope* stack, Scope scope){
    assert(scope != NULL);
    
    // check if the new scope's already on the stack
    Scope test = *stack;
    while(test != NULL){
        if(test == scope)
            return;
        test = test->next;
    }
    
    scope->next = (*stack);
    if(scope->variables == NULL)
        scope->variables = ((*stack) != NULL ? (*stack)->variables : NULL);
    else {
        Variable var = scope->variables;
        while(var->next != NULL)
            var = var->next;
        var->next = ((*stack) != NULL ? (*stack)->variables : NULL);
    }
    
    (*stack) = scope;
}

/*
 * Pop the top scope off the stack of active scopes
 */
void pop_scope_stack(Scope* stack){
    assert((*stack) != NULL);
    
    Scope scope = (*stack);
    if(scope->variables != NULL){
        Variable var = scope->variables;
        while(var->next != (scope->next != NULL ? scope->next->variables : NULL))
            var = var->next;
        var->next = NULL;
    }
    
    (*stack) = scope->next;
    scope->next = NULL;
}

/*
 * If the AST declares a path (PATH) or a scope (EXPAND)
 * create the appropriate record
 */
void preevaluate_AST(Token tokens, LineType line, Path* pathList, SScope* scopeList){
    if(tokens == NULL)
        return;
    
    // we are guaranteed by create_AST that the AST
    // has a builtin as its head and that it is
    // well-formed for the given builtin
    if(tokens->builtin == PATH) // make a path
        *pathList = make_path(tokens->left->name, line, *pathList);
    else if(tokens->builtin == EXPAND) // make a scope
        *scopeList = make_sscope(line, make_scope(NULL), *scopeList);
    // for all other builtins we don't need to do anything
}

/*
 * Recursively evaluate the given AST in the given context
 * If a BIFURC occured, sets nextLine to indicate the jump target
 * Returns the value of the AST
 */
ValueType evaluate_AST(Token tokens, LineType line, Scope* scopeStack, Path pathList, SScope scopeList, LineType* nextLine){
    assert(nextLine != NULL);
    
    if(tokens == NULL)
        return DEFAULT_AST_VALUE;
    
    // we are guaranteed by create_AST that the AST
    // is well-formed, so we will NOT be syntax-checking
    
    // base cases
    if(tokens->type == LITERAL)
        return tokens->literal;
    if(tokens->type == NAME)
        return get_variable(*scopeStack, tokens->name);
    // non-base cases
    switch(tokens->builtin){
        case EXPAND:
            push_scope_stack(scopeStack, get_scope(scopeList, line));
            return DEFAULT_AST_VALUE;
        case COLLAPSE:
            pop_scope_stack(scopeStack);
            return DEFAULT_AST_VALUE;
        case INIT:
            create_variable(*scopeStack, tokens->left->name, DEFAULT_VARIABLE_VALUE);
            return DEFAULT_AST_VALUE;
        case TERM:
            delete_variable(*scopeStack, tokens->left->name);
            return DEFAULT_AST_VALUE;
        case SET:
            set_variable(*scopeStack, tokens->left->name, EVAL_RIGHT);
            return DEFAULT_AST_VALUE;
        case BIFURC:
            *nextLine = (EVAL_LEFT ? get_path(pathList, tokens->right->left->name) : get_path(pathList, tokens->right->right->name));
            return DEFAULT_AST_VALUE;
        case OUTN:
            printf("%i", EVAL_LEFT);
            return DEFAULT_AST_VALUE;
        case OUTC:
            printf("%c", EVAL_LEFT % MAX_PRINTABLE_CHAR);
            return DEFAULT_AST_VALUE;
        // do we want to be helpful here and return 0xffffffff for true, so NEG works better?
        case AND:
            return EVAL_LEFT && EVAL_RIGHT;
        case OR:
            return EVAL_LEFT || EVAL_RIGHT;
        case ADD:
            return EVAL_LEFT + EVAL_RIGHT;
        case SUBTRACT:
            return EVAL_LEFT - EVAL_RIGHT;
        case MULTIPLY:
            return EVAL_LEFT * EVAL_RIGHT;
        case DIVIDE:
            return EVAL_LEFT / EVAL_RIGHT;
        case L_SHIFT:
            return EVAL_LEFT << EVAL_RIGHT;
        case R_SHIFT:
            return EVAL_LEFT >> EVAL_RIGHT;
        case LESS_THAN:
            return EVAL_LEFT < EVAL_RIGHT;
        case GREATER_THAN:
            return EVAL_LEFT > EVAL_RIGHT;
        case BIT_AND:
            return EVAL_LEFT & EVAL_RIGHT;
        case BIT_OR:
            return EVAL_LEFT | EVAL_RIGHT;
        case NEG:
            return ~ EVAL_LEFT; // bitwise not
        default:
            // ie PATH; L_PAREN and R_PAREN are stripped by create_AST()
            return DEFAULT_AST_VALUE;
    }
}