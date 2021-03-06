#include "lex.h"

// for Names
const unsigned INITIAL_NAME = 0;

// for token boundary finding
const int EOL = -1;

// for lexing
const char COMMENT_CHAR = ';';
const int NUM_BUILTINS = 25;
char* builtin_strs[NUM_BUILTINS] = {"expand", "collapse", "init", "term", "set", "path", "bifurc", "outn",  "outc", "in", "and", "or", "+", "-", "*", "/", "<<", ">>", "<", ">", "&", "|", "!", "(", ")"};


/*
 * Create a new NameRec from the given values
 * Returns a new NameRec with the given values
 */
Name make_name(char* fullName, NameType name, Name next){
    assert(fullName != NULL);
    Name retVal = (Name) malloc(sizeof(NameRec));
    assert(retVal != NULL);
    retVal->fullName = (FullNameType) malloc(strlen(fullName) + 1);
    strncpy(retVal->fullName, fullName, strlen(fullName) + 1);
    retVal->name = name;
    retVal->next = next;
    return retVal;
}

/*
 * Frees a Name, including its internal char*
 */
void free_name(Name* name){
    assert(*name != NULL);
    free((*name)->fullName);
    free(*name);
    *name = NULL;
}

/*
 * If no record for fullName exists in nameList, append one
 * Returns the name for the given fullName
 */
NameType add_name(Name* nameList, char* fullName){
    assert(fullName != NULL);
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

/*
 * Make a token of the appropriate type with the given value
 * Hooks up prev to it if given
 */
Token make_builtin_token(BuiltinType builtin, Token prev){
    Token retVal = (Token) malloc(sizeof(TokenRec));
    assert(retVal != NULL);
    retVal->type = BUILTIN;
    retVal->builtin = builtin;
    retVal->next = NULL;
    retVal->left = NULL;
    retVal->right = NULL;
    if(prev != NULL)
        prev->next = retVal;
    
    return retVal;
}

Token make_name_token(NameType name, Token prev){
    Token retVal = (Token) malloc(sizeof(TokenRec));
    assert(retVal != NULL);
    retVal->type = NAME;
    retVal->name = name;
    retVal->next = NULL;
    retVal->left = NULL;
    retVal->right = NULL;
    if(prev != NULL)
        prev->next = retVal;
    return retVal;
}

Token make_literal_token(LiteralType literal, Token prev){
    Token retVal = (Token) malloc(sizeof(TokenRec));
    assert(retVal != NULL);
    retVal->type = LITERAL;
    retVal->literal = literal;
    retVal->next = NULL;
    retVal->left = NULL;
    retVal->right = NULL;
    if(prev != NULL)
        prev->next = retVal;
    return retVal;
}

/*
 * Frees a token
 */
void free_token(Token* token){
    assert(*token != NULL);
    free(*token);
    *token = NULL;
}

/*
 * Get the index of the end of the current token
 * Returns the index of the first whitespace character after start_pos
 */
int get_token_end(char* input, int start_pos){
    int input_len = strlen(input);
    
    if(start_pos >= input_len)
        return EOL;
    
    int pos = start_pos;
    while((input[pos] != COMMENT_CHAR) && (pos < input_len)){
        if((input[pos] == ' ') || (input[pos] == '\t') || (input[pos] == '\n'))
            return pos;
        pos++;
    }
    
    return pos;
}

/*
 * Get the index of the start of the next token 
 * Returns the next non-whitespace character after start_pos
 */
int get_token_start(char* input, int start_pos){
    int input_len = strlen(input);
    
    if(start_pos >= input_len)
        return EOL;
    
    int pos = start_pos;
    while((input[pos] != COMMENT_CHAR) && (pos < input_len)){
        if((input[pos] != ' ') && (input[pos] != '\t') && (input[pos] != '\n'))
            return pos;
        pos++;
    }
    
    return EOL;
}

/*
 * Tokenizes an input string
 * Returns a list of tokens and updates the given list of names
 * with the new names in the token stream
 */
void tokenize_string(char* input, Token* tokens, Name* names){
    assert(*tokens == NULL); // you must give an empty list of tokens
    
    int token_start = 0;
    int token_end = 0;
    int token_len = EOL;

    Token lastToken = *tokens;
    
    while(1){
        token_start = get_token_start(input, token_end);
        token_end = get_token_end(input, token_start);
        token_len = token_end - token_start;
        
        if(token_start == EOL) // we've hit the end of line, so we're done
            break;
        
        assert(token_len > 0);
        
        char* tok_start = input + token_start;
        char* tok_str = (char*) malloc(token_len + 1);
        
        strncpy(tok_str, tok_start, token_len);
        tok_str[token_len] = '\0';
        
        Token newToken = NULL;
        // first check is to disambugate negative numbers from MINUS
        if((tok_str[0] == '-' && strlen(tok_str) > 1) || (tok_str[0] >= '0' && tok_str[0] <= '9')){ // literal
            for(char* c = tok_str + 1; *c != '\0'; c++)
                if(*c < '0' || *c > '9')
                    ERROR("invalid characters for value");
            newToken = make_literal_token(atoi(tok_str), lastToken);
        } else { // builtin
            for(int i = 0; i < NUM_BUILTINS; i++){
                if(strcmp(builtin_strs[i], tok_str) == 0){
                    newToken = make_builtin_token(i, lastToken);
                    break;
                }
            }
        }
        if(newToken == NULL){ // wasn't literal or builtin, so must be name
            // legal characters are A-Za-z0-9_
            for(char* c = tok_str; *c != '\0'; c++)
                if(!((*c == '_') || (*c >= '0' && *c <= '9') || (*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z')))
                    ERROR("invalid characters for name");
            newToken = make_name_token(add_name(names, tok_str), lastToken);
        }
        
        if(*tokens == NULL)
            *tokens = newToken;
        lastToken = newToken;
        
        free(tok_str);
    }
    
    
}
