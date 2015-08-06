#ifndef _test_defs_h
#define _test_defs_h

#include <stdio.h>
#include <assert.h>

// defines for use in all test files
// put here in and not in test.h to avoid circular includes

#define TEST_GROUP_INDICATOR(x) printf("\t\tTesting %s...\n", x);
#define TEST_FILE_START_INDICATOR(x) printf("\tBeginning %s tests\n", x);
#define TEST_FILE_END_INDICATOR(x) printf("\tAll %s tests passed!\n", x);

#endif
