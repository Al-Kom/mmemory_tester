#include <stdio.h>
#include <stdlib.h>
#include "mmemory.h"

//----------------------------------------------------------------------------------
// test functions
//----------------------------------------------------------------------------------

int test_init()
{
    if( initialize(3,5)  ||       //positive test
       !initialize(-2,6) ||       //negative test (and next)
       !initialize(4,-7) ||
       !initialize(-3,-7))
        return 1;
    return 0;
}

//----------------------------------------------------------------------------------
// main
//----------------------------------------------------------------------------------

int main()
{
    printf("Hello world!\n");
    printf("test initialize: \t\t%s\n", test_init()? "FAIL":"PASS");
    return 0;
}
