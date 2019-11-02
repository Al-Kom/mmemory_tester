#include "mmemory.h"
#include <stdlib.h>

/// TODO (alkom#6#11/02/19): make more DRY _malloc()
/// TODO (alkom#4#11/02/19): add checking to null to all malloc() using

//----------------------------------------------------------------------------------
// memory structures
//----------------------------------------------------------------------------------

typedef struct Block
{
    char*              data;    //pointer to user data

    size_t             size;    //size of user data

    struct Block*      next;    //pointer to next memory block

} Block;

typedef struct MemoryManager
{
    char*			memory;			//memory to get user

    size_t			szMemory;		//maximal size of memory

    struct Block*	zero_block;	    //pointer to first memory block

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
            *current_block = manager.zero_block,
            *next_block = 0;
    while(current_block)
    {
        next_block = current_block->next;
        free(current_block);
        current_block = next_block;
    }
    manager.zero_block = 0;
}


 /**
 	@brief	Create Block entity and initialize it with input values

	@param	[in] data           pointer to user data
	@param	[in] data_size      size of user data
	@param	[in] next           pointer to first memory block

	@return	pointer to Block (0 if error)
 **/
Block* create_block(char* data, size_t data_size, Block* next)
{
    Block* res = malloc(sizeof(Block));
    if(!res)
        return 0;
    res->data = data;
    res->size = data_size;
    res->next = next;
    return res;
}
//----------------------------------------------------------------------------------
// library functions
//----------------------------------------------------------------------------------

int _malloc (VA* ptr, size_t szBlock)
{
    //check initialization
    if(!is_inited)
        return -2;
    //verify arguments
    if(!ptr ||                          //non-valid address
       ((long long)szBlock <= 0))       //or undefined behavior
        return -1;
    //search free space
    *ptr = 0;
    if(!manager.zero_block)    //no blocks exist
    {
        if(szBlock <= manager.szMemory) //enough memory
        {
            //create and initialize start of blocks stack
            manager.zero_block = create_block(manager.memory, 0, 0);
            if(!manager.zero_block)
                return 1;
            //create and initialize first block
            manager.zero_block->next = create_block(manager.memory, szBlock, 0);
            if(!manager.zero_block->next)
                return 1;
            *ptr = manager.memory;  //return address to user
            return 0;
        }
        else
            return -2;  //not enough memory
    }//if(!manager.zero_block)
    else                       //at least two (zero and first) blocks exist
    {
        //search free space between two blocks
        Block   *cur_block = manager.zero_block,
                *next_block = manager.zero_block->next;
        for(char *cur_end = 0, *next_start = 0;
            next_block;
            cur_block = next_block, next_block = next_block->next)
        {
            cur_end = cur_block->data + cur_block->size;
            next_start = next_block->data;
            if(cur_end + szBlock < next_start)      //enough memory to insert new
            {                                       //block between two blocks
                //create and initialize new block
                cur_block->next = create_block(cur_end, szBlock, next_block);
                if(!cur_block)
                    return 1;
                *ptr = manager.memory;  //return address to user
                return 0;
            }
        }
        //no enough space between two blocks, search space after last block
        //current_block now is the last block
        char* last_end = cur_block->data + cur_block->size;
        if(last_end + szBlock < manager.memory + manager.szMemory)  //enough memory
        {                                                           //to add
            //create and initialize new block
            cur_block->next = create_block(last_end, szBlock, next_block);
            if(!cur_block)
                return 1;
            *ptr = manager.memory;      //return address to user
            return 0;
        }
        else
            return -2;          //not enough memory
    }//else(!manager.zero_block)
    return 1;    //unreachable
}

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
    manager.memory = malloc(manager.szMemory);
    if(!manager.memory)
    {
        free_manager();
        return 1;
    }

    is_inited = 1;
    return 0;
}
