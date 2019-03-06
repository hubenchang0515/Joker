#include "mempool.h"
#include <string.h>

static void MemoryPoolSetError(mPool_t* pool, const char* error);
static mBlock_t* MemoryPoolAddBlock(mPool_t* pool);

static mBlock_t* MemoryBlockFindAvailable(mBlock_t* block);
static void* MemoryBlockAlloc(mPool_t* pool, mBlock_t* block);

static mBlock_t* MemoryAddressFindBlock(mPool_t* pool, void* addr);
static void MemoryAddressFree(mPool_t* pool, mBlock_t* block, void* addr);

/* Return error message */
const char* MemoryPoolError(mPool_t* pool)
{
    return pool->error;
}

/* Create a memory pool */
int MemoryPoolCreate(mPool_t* pool, size_t unitSize, size_t unitCount)
{
    pool->unitSize = unitSize;
    pool->unitCount = unitCount;
    pool->firstBlock = NULL;
    pool->lastBlock = NULL;

    return EXIT_SUCCESS;
}

/* Alloc memory */
void* MemoryPoolAlloc(mPool_t* pool, size_t size)
{
    if(size > pool->unitSize)
    {
        MemoryPoolSetError(pool, "needed size is bigger than unit size.");
        return NULL;
    }

    mBlock_t* block = pool->lastBlock;
    if(block == NULL || block->available == 0)
    {
        block = MemoryPoolAddBlock(pool);
        if(block == NULL)
        {
            return NULL;
        }
    }

    return MemoryBlockAlloc(pool, block);
}

/* Free memory */
int MemoryPoolFree(mPool_t* pool, void* addr)
{
    mBlock_t* block = MemoryAddressFindBlock(pool, addr);
    if(block == NULL)
    {
        MemoryPoolSetError(pool, "address not in this pool.");
        return EXIT_FAILURE;
    }
    MemoryAddressFree(pool, block, addr);
    return EXIT_SUCCESS;
}

/* Destroy memory pool */
void MemoryPoolDestroy(mPool_t* pool)
{
    mBlock_t* current = NULL;
    mBlock_t* next = pool->firstBlock;
    while(next != NULL)
    {
        current = next;
        next = current->nextBlock;
        free(current);
    }
}











/*****************************************************************
* Description : Set error message 
* Parsms      : pool - memory pool
*               error - error message
* Return      : void
*****************************************************************/
static void MemoryPoolSetError(mPool_t* pool, const char* error)
{
    strncpy(pool->error, error, JOCKER_MEMPOOL_ERROR_SIZE);
}


/*****************************************************************
* Description : Add a new block to memory pool
* Parsms      : pool - memory pool
* Return      : SUCCESS - the new memory block
                FALIURE - NULL
*****************************************************************/
static mBlock_t* MemoryPoolAddBlock(mPool_t* pool)
{
    size_t blockSize = sizeof(mBlock_t) + pool->unitCount * (sizeof(mUnit_t) + pool->unitSize);
    mBlock_t* mem = (mBlock_t*)malloc(blockSize);
    if(mem == NULL)
    {
        MemoryPoolSetError(pool, "malloc system call failure");
        return NULL;
    }
    else
    {
        memset(mem, 0, blockSize);
        mem->available = pool->unitCount;
        mem->firstUnit = 0;
        mem->nextBlock = NULL;
        if(pool->firstBlock == NULL)
        {
            pool->firstBlock = mem;
            pool->lastBlock = mem;
        }
        else
        {
            mem->prevBlock = pool->lastBlock;
            pool->lastBlock->nextBlock = mem;
            pool->lastBlock = mem;
        }
        
        return mem;
    }
}


/*****************************************************************
* Description : Find a memory block has available unit
* Parsms      : pool - memory pool
* Return      : SUCCESS - the memory block has has available unit 
                FALIURE - NULL
*****************************************************************/
static mBlock_t* MemoryBlockFindAvailable(mBlock_t* block)
{
    for(; block != NULL; block = block->prevBlock)
    {
        if(block->available > 0)
        {
            return block;
        }
    }

    return NULL;
}

/*****************************************************************
* Description : Alloc a unit from block
* Parsms      : pool - memory pool
*               block - memory block
* Return      : SUCCESS - memory address
                FALIURE - NULL
*****************************************************************/
static void* MemoryBlockAlloc(mPool_t* pool, mBlock_t* block)
{
    uint8_t* index = (uint8_t*)block->datas;
    uint8_t* memory  = ((uint8_t*)block->datas) + pool->unitCount;
    ptrdiff_t unitSize = pool->unitSize;
    size_t pos = block->firstUnit;
    void* use = (void*)(memory + pos * unitSize);
    index[pos] = 1;

    block->available -= 1;
    block->firstUnit += 1;
    // for(size_t i = 0; block->available > 0 && i < pool->unitCount; i++)
    // {
    //     if(index[i] == 0)
    //     {
    //         block->firstUnit = i;
    //         break;
    //     }
    // }
    
    return use;
}



/*****************************************************************
* Description : Find the address belong to whitch block
* Parsms      : pool - memory pool
*               addr - memory address
* Return      : SUCCESS - memory block own the address
                FALIURE - NULL
*****************************************************************/
static mBlock_t* MemoryAddressFindBlock(mPool_t* pool, void* addr)
{
    size_t blockSize = sizeof(mBlock_t) + pool->unitCount * (sizeof(mUnit_t) + pool->unitSize);
    mBlock_t* block = pool->firstBlock;
    for(; block != NULL; block = block->nextBlock)
    {
        if((uint8_t*)addr > (uint8_t*)block && (uint8_t*)addr < ((uint8_t*)block) + blockSize)
        {
            return block;
        }
    }
    return NULL;
}


/*****************************************************************
* Description : Free the memory address
* Parsms      : pool - memory pool
*               block - memory block own the address
*               addr - memory address
* Return      : void
*****************************************************************/
static void MemoryAddressFree(mPool_t* pool, mBlock_t* block, void* addr)
{
    uint8_t* index = (uint8_t*)block->datas;
    uint8_t* memory  = ((uint8_t*)block->datas) + pool->unitCount;
    size_t pos = ((uint8_t*)addr - memory) / pool->unitSize;

    index[pos] = 0;
    block->available += 1;
    if(block->firstUnit > pos)
    {
        block->firstUnit = pos;
    }

    // free empty block
    if(block->available == pool->unitCount)
    {
        if(block->prevBlock == NULL)
        {
            pool->firstBlock = block->nextBlock;
        }
        else
        {
            block->prevBlock->nextBlock = block->nextBlock;
        }


        if(block->nextBlock == NULL)
        {
            pool->lastBlock = block->prevBlock;
        }
        else
        {
            block->nextBlock->prevBlock = block->prevBlock;
        }

        free(block);
    }
}