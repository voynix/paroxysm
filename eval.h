#ifndef ____eval__
#define ____eval__

#include <stdio.h>
#include <assert.h>
#include "lex.h"

typedef LiteralType ValueType;

typedef struct VariableRec {
    NameType name;
    ValueType value;
    struct VariableRec* next;
} VariableRec;
typedef struct VariableRec* Variable;

typedef struct ScopeRec {
    unsigned numVars;
    Variable variables;
    struct ScopeRec* next;
} ScopeRec;
typedef struct ScopeRec* Scope;

//Variable make_variable(NameType name, ValueType value);
//void free_variable(Variable* variable);
//
//Scope make_scope(Scope next);
//void free_scope(Scope* scope); // compare variables to next->variables when freeing variables to know when to stop
//void create_variable(Scope scope, NameType name, ValueType value);
//void set_variable(Scope scopeList, NameType name, ValueType value); // chain varLists across scopes!
//ValueType get_variable(Scope scopeList, NameType name);

#endif
