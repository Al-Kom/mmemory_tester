#include <stdio.h>
#include <stdlib.h>
#include "mmemory.h"

//----------------------------------------------------------------------------------
// test functions
//----------------------------------------------------------------------------------

int test_init()
{
    if( initialize(3,5)    ||
       !(initialize(-2,6)) ||
       !(initialize(4,-7)) ||
       !(initialize(-3,-7))||
       )
        return 1;
    return 0;
}

//----------------------------------------------------------------------------------
// main
//----------------------------------------------------------------------------------

int main()
{
    printf("Hello world!\n");
    printf("test initialize: %s\n", test_init()? "NOT pass":"pass");
    return 0;
}
