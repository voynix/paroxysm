#ifndef ____lex__
#define ____lex__

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef enum {BUILTIN, NAME, LITERAL} TokenType;
typedef enum {EXPAND, COLLAPSE, INIT, TERM, SET, PATH, BIFURC, OUTN, OUTC, AND, OR, ADD, SUBTRACT, MULTIPLY, DIVIDE, L_SHIFT, R_SHIFT, LESS_THAN, GREATER_THAN, BIT_AND, BIT_OR, NEG, L_PAREN, R_PAREN} BuiltinType;
typedef unsigned NameType;
typedef int LiteralType;
typedef char* FullNameType;

typedef struct TokenRec {
    TokenType type;
    union {
        BuiltinType builtin;
        NameType name;
        LiteralType literal;
    };
    struct TokenRec* next;
    struct TokenRec* left;
    struct TokenRec* right;
} TokenRec;
typedef struct TokenRec* Token;

typedef struct NameRec {
    FullNameType fullName;
    NameType name;
    struct NameRec* next;
} NameRec;
typedef struct NameRec* Name;

Name make_name(char* fullName, NameType name, Name next);
void free_name(Name* name);
NameType add_name(Name* nameList, char* fullName);

Token make_builtin_token(BuiltinType builtin, Token prev);
Token make_name_token(NameType name, Token prev);
Token make_literal_token(LiteralType literal, Token prev);

void free_token(Token* t);

int get_token_end(char* input, int start_pos);
int get_token_start(char* input, int start_pos);

void tokenize_string(char* input, Token* tokens, Name* names);

#endif
