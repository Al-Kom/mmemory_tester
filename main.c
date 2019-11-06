#include <stdio.h>
#include <stdlib.h>
#include "mmemory.h"

//----------------------------------------------------------------------------------
// unit test functions
//----------------------------------------------------------------------------------

int test_init(void)
{
    //positive test
    if(initialize(3,5))
        return 1;
    //negative tests
    if(!initialize(-2,6)    ||
       !initialize(4,-7)    ||
       !initialize(-3,-7)   ||
       !initialize(0,4))
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
    initialize(3,3);
        //test with bad parameters
    if(!_malloc(&ptr, -3)   ||
       !_malloc(&ptr, 0)    ||
       !_malloc(0, 5)       ||
       !_malloc(0, 0))
        return 1;
    return 0;
}


int test_free(void)
{
    VA ptr;
    //positive tests
        //test ...
    initialize(3,3);
    _malloc(&ptr,4);
    if(_free(ptr))
        return 1;
    //negative tests
        //test with bad parameter
    _malloc(&ptr,4);
    if(!_free(0)        ||
       !_free(ptr + 1)  ||
       !_free(-1))
        return 1;
    return 0;
}

int test_write(void)
{
    VA ptr;
    char* buffer = "Hello";
    int szBuffer = 5;
    //-----positive tests-----
        //test writing in first block
    initialize(3,3);
    _malloc(&ptr,6);
    if(_write(ptr, buffer, szBuffer))
        return 1;
    //-----negative tests-----
        //test bad parameters
    if(!_write(0, buffer, szBuffer)   ||
       !_write(ptr, 0, szBuffer)     ||
       !_write(ptr, buffer, -1))
        return 1;
    return 0;
}

//----------------------------------------------------------------------------------
// integrated test functions
//----------------------------------------------------------------------------------

int integr_test_malloc_free(void)
{
    VA ptr1, ptr2, ptr3, ptr4;
    //-----positive tests-----
    //test allocation after last block
    if(initialize(3,3)      ||
        _malloc(&ptr1, 3)   ||
        _malloc(&ptr2, 1))
        return 1;
    //test free block between first and third blocks
    if(_malloc(&ptr3,3)   ||
       _free(ptr2))
        return 1;
    //test add after last, cause space between first and third is too few (1b)
    if(_malloc(&ptr4,2))
        return 1;
    //test insert between first and third (1b), no space after last
    if(_malloc(&ptr2,1))
        return 1;
    //test insert between zero and first blocks
    if(_free(ptr1)  ||
       _malloc(&ptr1,2))
        return 1;
    //-----negative tests-----
    //test allocation of non-initialized memory
    initialize(-3,3);
    if(!_malloc(&ptr1, 4))
        return 1;
    //test "no enough memory" when no blocks exist
    if(initialize(3,3) ||
       !_malloc(&ptr1,10))
        return 1;
    //test "no enough memory" after last block
    if(initialize(3,3)      ||
       _malloc(&ptr1, 4)    ||
       !_malloc(&ptr2,10))
        return 1;
    //test bad insert between two blocks when no space after
    if(initialize(3,3)  ||
       _malloc(&ptr1,3) ||
       _malloc(&ptr2,2) ||
       _malloc(&ptr3,4) ||
       _free(ptr2)      ||  //all free space (2b) is between ptr1 and ptr3
       !_malloc(&ptr2,3))   //but trying allocate 3b
        return 1;
    return 0;
}

int integr_test_read_write(void)
{
    ///TODO: add tests with _write and _read
    VA ptr1,ptr2;
    char* buffer = "Hello";
    int szBuffer = 5;
    //-----positive tests-----
    //test write in non-first block part of buffer
    if(initialize(3,3)  ||
       _malloc(&ptr1,4) ||
       _malloc(&ptr2,3) ||
       _write(ptr2,buffer,szBuffer/2))
       return 1;
    //-----negative tests-----
    //test write on non-initialized memory
    initialize(-3,0);
    _malloc(&ptr1,6);
    if(!_write(ptr1,buffer,szBuffer))
        return 1;
    //test write data with size more than block (overwriting)
    if(initialize(3,3)     ||
       _malloc(&ptr1,4)    ||
       !_write(ptr1,buffer,5))
        return 1;
    //test write in non-existing block
    if(!_write(ptr2,buffer,szBuffer))
        return 1;
    //test write in freed block
    if(_malloc(&ptr2,5) ||
       _free(ptr2)      ||
       !_write(ptr2,buffer,szBuffer))
        return 1;
    return 0;
}

//----------------------------------------------------------------------------------
// main
//----------------------------------------------------------------------------------

int main(void)
{
    printf("Hello world!\n");
    printf("test initialize: \t\t%s\n", test_init()? "FAIL":"PASS");
    printf("test memory allocation: \t%s\n", test_malloc()? "FAIL":"PASS");
    printf("test memory free: \t\t%s\n", test_free()? "FAIL":"PASS");
    printf("INTGR test allocation-free: \t%s\n",
           integr_test_malloc_free()? "FAIL":"PASS");
    printf("test writing: \t\t\t%s\n", test_write()? "FAIL":"PASS");
    printf("INTGR test read-write: \t\t%s\n",
           integr_test_read_write()? "FAIL":"PASS");
    return 0;
}
