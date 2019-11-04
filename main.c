#include <stdio.h>
#include <stdlib.h>
#include "mmemory.h"

//----------------------------------------------------------------------------------
// unit test functions
//----------------------------------------------------------------------------------

int test_init(void)
{
    if( initialize(3,5)  ||       //positive test
       !initialize(-2,6) ||       //negative test (and next)
       !initialize(4,-7) ||
       !initialize(-3,-7))
        return 1;
    return 0;
}

int test_malloc(void)
{
    VA ptr;
    //positive tests
        //test allocation when no blocks exist
    initialize(3,15);
    if(_malloc(&ptr, 4))
        return 1;
    //negative tests
         //test allocation of non-initialized memory
    initialize(-3,3);
    if(!_malloc(&ptr, 4))
        return 1;
    initialize(3,3);
        //test with bad parameters
    if(!_malloc(&ptr, -3) ||
       !_malloc(&ptr, 0)  ||
       !_malloc(0, 5)     ||
       !_malloc(0, 0))
        return 1;
    return 0;
}

//----------------------------------------------------------------------------------
// integrated test functions
//----------------------------------------------------------------------------------

int integr_test_malloc_free()
{
    VA ptr1, ptr2, ptr3;
    //positive tests
        //test allocation after last block
    if(initialize(3,3)      ||
       _malloc(&ptr1, 4)    ||
       _malloc(&ptr2, 2))
        return 1;
/*TODO (after _free adding):+test insert between two blocks
                            +test insert between zero and first blocks
                            +test bad insert with allocation after last*/
    //negative tests
        //test "no enough memory" when no blocks exist
    initialize(3,3);
    if(!_malloc(&ptr1,10))
        return 1;
        //test "no enough memory" after last block
    initialize(3,3);
    _malloc(&ptr1, 4);
    if(!_malloc(&ptr2,10))
        return 1;
/*TODO (after _free adding):-test bad insert between two blocks, not after
                            - -//- between zero and first blocks, not after*/
    return 0;
}

//----------------------------------------------------------------------------------
// main
//----------------------------------------------------------------------------------

int main(void)
{
    printf("Hello world!\n");
    //printf("test initialize: \t\t%s\n", test_init()? "FAIL":"PASS");
    printf("test memory allocation: \t%s\n", test_init()? "FAIL":"PASS");
    printf("intr. test malloc-free: \t%s\n",
           integr_test_malloc_free()? "FAIL":"PASS");
    return 0;
}
