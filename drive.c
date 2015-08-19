#include "drive.h"

void run_file(char* fileName){
    char* mode = "r";
    FILE *fp = fopen(fileName, mode);
    if(fp == NULL)
        assert(0); // TODO: proper error handling
    
    char* line = NULL;
    size_t len = 0;
    unsigned numLines = 0;
    while(getline(&line, &len, fp) != -1)
        numLines++;
    
    // if we got an empty file, do nothing
    if(numLines == 0)
        return;
    
    // make an array to hold all the ASTs
    Token* tokenizedLines = (Token*) malloc(numLines * sizeof(Token*));
    
    // rewind to beginning of file
    assert(fseek(fp, 0, SEEK_SET) == 0);
    
    // lex, parse, and preevaluate each line
    LineType currentLine = 0;
    Name nameList = NULL;
    Path pathList = NULL;
    SScope scopeList = NULL;
    Token tokens = NULL;
    while(getline(&line, &len, fp) != -1){
        tokens = NULL;
        tokenize_string(line, &tokens, &nameList);
        tokenizedLines[currentLine] = create_AST(tokens);
        preevaluate_AST(tokenizedLines[currentLine], currentLine, &pathList, &scopeList);
        currentLine++;
    }
    
    // close the file handle
    fclose(fp);
    
    // evaluate!
    currentLine = 0;
    LineType nextLine;
    Scope scopeStack = NULL;
    while(currentLine < numLines){
        nextLine = currentLine + 1;
        evaluate_AST(tokenizedLines[currentLine], currentLine, &scopeStack, pathList, scopeList, &nextLine);
        currentLine = nextLine;
    }
    
    // cleanup, lol?
}
