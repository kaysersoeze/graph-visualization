/*****************************************************************************

* File Name: test.c

* Author: Ludvig Sundström

* Description: 

* Creation Date: 05-07-2015

*****************************************************************************/

#include <stdio.h>

#include "minunit.h"

int tests_run = 0;
int foo = 7; 

static char *test_foo() {
    mu_assert("foo != 7", foo == 7);
    return 0;
}

static char *all_tests() {
    mu_run_test(test_foo);
    return 0;
}

int main(int argc, char **argv) {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
 
