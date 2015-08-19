#include "main.h"


int main(int argc, char** argv){
    assert(argc == 2); // TODO: proper error handling
    
    run_file(argv[1]);
    
    return 0;
}
