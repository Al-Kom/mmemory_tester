#include "mmemory.h"
#include <stdlib.h>

//----------------------------------------------------------------------------------
// memory structures
//----------------------------------------------------------------------------------

typedef struct Block
{
    VA*                data;    //pointer to user data

    size_t             size;    //size of user data

    struct Block*      next;    //pointer to next memory block
} Block;

typedef struct MemoryManager
{
    VA*				memory;			//memory to get user

    size_t			szMemory;		//maximal size of memory

    struct Block*	first_block;	//pointer to first memory block

} MemoryManager;

//----------------------------------------------------------------------------------
// global variables
//----------------------------------------------------------------------------------

int is_inited = 0;      // initialize flag

MemoryManager manager;  // global manager

//----------------------------------------------------------------------------------
// private functions
//----------------------------------------------------------------------------------

/**
 	@func	free_manager
 	@brief	Free memory, that was allocated  for manager, and clean data
**/
void free_manager(void)
{
    //free memory
    manager.szMemory = 0;
    if(manager.memory)
    {
        free(manager.memory);
        manager.memory = 0;
    }
    //free blocks
    Block
            *current_block = manager.first_block,
            *next_block = 0;
    while(current_block)
    {
        next_block = current_block->next;
        free(current_block);
        current_block = next_block;
    }
    manager.first_block = 0;
}

//----------------------------------------------------------------------------------
// library functions
//----------------------------------------------------------------------------------

int initialize(int n, int szPage)
{
    is_inited = 0;

    //clean manager
    free_manager();
    //check arguments
    if(n <= 0 || szPage <=0)
        return -1;
    //allocate user memory
    manager.szMemory = (size_t) n*szPage;
    manager.memory = malloc(sizeof(VA)*manager.szMemory);
    if(!manager.memory)
    {
        free_manager();
        return 1;
    }

    is_inited = 1;
    return 0;
}
