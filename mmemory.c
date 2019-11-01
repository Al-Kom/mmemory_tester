#include "mmemory.h"
/**
	@
**/

typedef struct Block
{
    VA*                data;    //pointer to user data

    size_t             size;    //size of user data

    struct Block*      next;    //pointer to next memory block
} Block;

struct MemoryManager
{
    VA*				memory;			//memory to get user

    size_t			szMemory;		//maximal size of memory

    struct Block*	first_block;	//pointer to first memory block

};

//	initialize flag
int is_inited = 0;
//	global manager
MemoryManager manager;
