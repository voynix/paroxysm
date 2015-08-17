#include "eval.h"

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
 * Push the given scope onto the stack of active scopes
 */
void push_scope_stack(Scope* stack, Scope scope){
    assert(scope != NULL);
    
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