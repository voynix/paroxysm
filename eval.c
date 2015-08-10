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
 * Creates a new ScopeRec with the given values
 * Returns the created ScopeRec
 */
Scope make_scope(Scope next){
    Scope retVal = (Scope) malloc(sizeof(ScopeRec));
    assert(retVal != NULL);
    retVal->numVars = 0;
    retVal->variables = NULL;
    retVal->next = next;
    return retVal;
}

/*
 * Frees a Scope, including all of its VariableRecs
 */
void free_scope(Scope* scope){
    Variable nextVar;
    while((*scope)->numVars > 0){
        assert((*scope)->variables != NULL);
        nextVar = (*scope)->variables->next;
        free((*scope)->variables);
        (*scope)->variables = nextVar;
        (*scope)->numVars--;
    }
    assert((*scope)->variables == NULL);
    free(*scope);
    *scope = NULL;
}