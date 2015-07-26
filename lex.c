#include "lex.h"

const unsigned int INITIAL_NAME = 0;
const char COMMENT_CHAR = ';';

/*
 * Create a new NameRec from the given values
 * Returns a new NameRec with the given values
 */
Name make_name(char* fullName, NameType name, Name next){
    Name retVal = (Name) malloc(sizeof(NameRec));
    assert(retVal != NULL);
    retVal->fullName = fullName;
    retVal->name = name;
    retVal->next = next;
    return retVal;
}

/*
 * If no record for fullName exists in nameList, append one
 * Returns the name for the given fullName
 */
NameType add_name(Name* nameList, char* fullName){
    Name list = *nameList;
    while(list != NULL)
        if(strcmp(fullName, list->fullName) == 0)
            return list->name;
        else
            list = list->next;
    NameType newName;
    if(*nameList == NULL)
        newName = INITIAL_NAME;
    else
        newName = (*nameList)->name + 1;
    *nameList = make_name(fullName, newName, *nameList);
    return (*nameList)->name;
}

Token make_builtin_token(BuiltinType builtin, Token next){
    Token retVal = (Token) malloc(sizeof(TokenRec));
    assert(retVal != NULL);
    retVal->type = BUILTIN;
    retVal->builtin = builtin;
    retVal->next = next;
    return retVal;
}

Token make_name_token(NameType name, Token next){
    Token retVal = (Token) malloc(sizeof(TokenRec));
    assert(retVal != NULL);
    retVal->type = NAME;
    retVal->name = name;
    retVal->next = next;
    return retVal;
}

Token make_literal_token(LiteralType literal, Token next){
    Token retVal = (Token) malloc(sizeof(TokenRec));
    assert(retVal != NULL);
    retVal->type = LITERAL;
    retVal->literal = literal;
    retVal->next = next;
    return retVal;
}

int get_token_end(char* input, int start_pos){
    int input_len = strlen(input);
    
    if(start_pos >= input_len)
        return -1;
    
    int pos = start_pos;
    while((intput[pos] != COMMENT_CHAR) && (pos < input_len)){
        if((input[pos] == ' ') || (input[pos] == '\t'))
            return pos;
        pos++;
    }
    return pos;
}

int get_token_start(char* input, int start_pos){
    int input_len = strlen(input);
    
    if(start_pos <= input_len)
        return -1;
    
    int pos = start_pos;
    while((intput[pos] != COMMENT_CHAR) && (pos < input_len)){
        if((input[pos] != ' ') && (input[pos] != '\t'))
            return pos;
        pos++;
    }
    return -1;
}

Token tokenize_string(char* input){
    int pos = 0;
    int posMax = strlen(input);
    
    
}
