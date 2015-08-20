#include "main.h"


int main(int argc, char** argv){
    ERROR_UNLESS(argc == 2, "USAGE: paroxysm FILE");
    
    run_file(argv[1]);
    
    return 0;
}
