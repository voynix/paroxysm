#ifndef ____eval__
#define ____eval__

#include <stdio.h>
#include <assert.h>
#include "lex.h"

typedef LiteralType ValueType;
typedef unsigned LineType;

typedef struct VariableRec {
    NameType name;
    ValueType value;
    struct VariableRec* next;
} VariableRec;
typedef struct VariableRec* Variable;

typedef struct PathRec {
    NameType name;
    LineType line;
    struct PathRec* next;
} PathRec;
typedef struct PathRec* Path;

// for the stack of active scopes
typedef struct ScopeRec {
    Variable variables;
    struct ScopeRec* next;
} ScopeRec;
typedef struct ScopeRec* Scope;

// for the list of all scopes
typedef struct ScopeRecRec {
    LineType line;
    Scope scope;
    struct ScopeRecRec* next;
} ScopeRecRec;
typedef struct ScopeRecRec* SScope;

Variable make_variable(NameType name, ValueType value, Variable next);
void free_variable(Variable* variable);

Path make_path(NameType name, LineType line, Path next);
void free_path(Path* path);

Scope make_scope(Scope next);
void free_scope(Scope* scope);

SScope make_sscope(LineType line, Scope scope, SScope next);
void free_sscope(SScope* sscope);

void create_variable(Scope scope, NameType name, ValueType value);
void set_variable(Scope scope, NameType name, ValueType value);
ValueType get_variable(Scope scopeList, NameType name);
void delete_variable(Scope scope, NameType name);

LineType get_path(Path pathList, NameType name);

void push_scope_stack(Scope* stack, Scope scope);
void pop_scope_stack(Scope* stack);


#endif
