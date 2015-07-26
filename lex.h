#ifndef ____lex__
#define ____lex__

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef enum {BUILTIN, NAME, LITERAL} TokenType;
typedef enum {EXPAND, COLLAPSE, INIT, NIHIL, SET, PATH, BIFURC, OUTN, OUTC, AND, OR, PLUS, MINUS, MULTIPLY, DIVIDE, SHIFT_LEFT, SHIFT_RIGHT, BIT_AND, BIT_OR, NEG, L_PAREN, R_PAREN} BuiltinType;
typedef unsigned int NameType;
typedef int LiteralType;

typedef struct TokenRec {
    TokenType type;
    union {
        BuiltinType builtin;
        NameType name;
        LiteralType literal;
    };
    struct TokenRec* next;
} TokenRec;
typedef struct TokenRec* Token;

typedef struct NameRec {
    char* fullName;
    NameType name;
    struct NameRec* next;
} NameRec;
typedef struct NameRec* Name;

Name make_name(char* fullName, NameType name, Name next);
NameType add_name(Name* nameList, char* fullName);

Token make_builtin_token(BuiltinType builtin, Token next);
Token make_name_token(NameType name, Token next);
Token make_literal_token(LiteralType literal, Token next);

int get_token_end(char* input, int start_pos);
void skip_whitespace(char* input, int* pos);

Token tokenize_string(char* input);

#endif