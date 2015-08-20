#ifndef ____error__
#define ____error__

#include <stdio.h>
#include <stdlib.h>

#define ERROR_UNLESS(x, y) if(!(x)) error(y);
#define ERROR(x) error(x);

void error(char* message);

#endif
