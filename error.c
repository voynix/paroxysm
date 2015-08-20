#include "error.h"

void error(char* message){
    printf("ERROR: %s\n", message);
    exit(1);
}
