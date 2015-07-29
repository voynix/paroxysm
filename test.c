#include "test.h"

int main(int argc, char* argv[]){
    printf("TESTING START\n");
    
    run_lexer_tests();
    run_parser_tests();
    
    printf("ALL TESTS PASSED!\n");
}