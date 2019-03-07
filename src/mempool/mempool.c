#include "mempool.h"
#include <string.h>
#include <stdio.h>

static void MemoryPoolSetError(mPool_t* pool, const char* error);
static mBlock_t* MemoryPoolAddBlock(mPool_t* pool);

static void* MemoryPoolAllocFromBlock(mPool_t* pool, mBlock_t* block);
static void* MemoryPoolFreeFromBlock(mPool_t* pool, mBlock_t* block, void* addr);

static void MemoryPoolInsertTotalBlockList(mPool_t* pool, mBlock_t* block);
static void MemoryPoolRemoveTotalBlockList(mPool_t* pool, mBlock_t* block);
static void MemoryPoolInsertAvailableBlockList(mPool_t* pool, mBlock_t* block);
static void MemoryPoolRemoveAvailableBlockList(mPool_t* pool, mBlock_t* block);

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
    pool->firstAvailableBlock = NULL;
    pool->lastAvailableBlock = NULL;

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

    mBlock_t* block = pool->firstAvailableBlock;
    if(block == NULL || block->availableUnitCount == 0)
    {
        block = MemoryPoolAddBlock(pool);
        if(block == NULL)
        {
            return NULL;
        }
    }

    return MemoryPoolAllocFromBlock(pool, block);
}

/* Free memory */
int MemoryPoolFree(mPool_t* pool, void* addr)
{
    if(addr == NULL)
    {
        MemoryPoolSetError(pool, "free NULL.");
        return EXIT_FAILURE;
    }

    mUnit_t* unit = (mUnit_t*)((uint8_t*)(addr) - sizeof(mUnit_t));
    mBlock_t* block = unit->block;
    size_t elementSize = sizeof(mUnit_t) + pool->unitSize;
    block->lastAvailable = (((uint8_t*)unit) - ((uint8_t*)(block->units))) / elementSize;
    block->availableUnitCount += 1;

    // insert into available block link-list
    if(block->availableUnitCount == 1)
    {
        MemoryPoolInsertAvailableBlockList(pool, block);
    }

    // remove empty block
    if(block->availableUnitCount == pool->unitCount)
    {
        MemoryPoolRemoveAvailableBlockList(pool, block);
        MemoryPoolRemoveTotalBlockList(pool, block);
        free(block);
    }

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


/* Debug */
void MemoryPoolDebug(mPool_t* pool)
{
    for(mBlock_t* iter = pool->firstBlock; iter != NULL; iter = iter->nextBlock)
    {
        printf("Block : %p \n", iter);
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
                FAILURE - NULL
*****************************************************************/
static mBlock_t* MemoryPoolAddBlock(mPool_t* pool)
{
    size_t blockSize = sizeof(mBlock_t) + pool->unitCount * (sizeof(mUnit_t) + pool->unitSize);
    mBlock_t* block = (mBlock_t*)malloc(blockSize);
    if(block == NULL)
    {
        MemoryPoolSetError(pool, "malloc system call failure");
        return NULL;
    }
    else
    {
        // init block
        block->availableUnitCount = pool->unitCount;
        block->firstAvailable = 0;
        block->lastAvailable = pool->unitCount - 1;
        block->prevBlock = NULL;
        block->nextBlock = NULL;
        block->prevAvailableBlock = NULL;
        block->nextAvailableBlock = NULL;

        // init units
        size_t elementSize = sizeof(mUnit_t) + pool->unitSize;
        for(size_t i = 0; i < pool->unitCount; i++)
        {
            mUnit_t* unit = (mUnit_t*)(block->units + i * elementSize);
            unit->nextAvailable = i + 1;
            unit->block = block;
        }

        MemoryPoolInsertTotalBlockList(pool, block);
        MemoryPoolInsertAvailableBlockList(pool, block);
        
        return block;
    }
}




/*****************************************************************
* Description : Alloc a unit from block
* Parsms      : pool - memory pool
*               block - memory block
* Return      : SUCCESS - memory address
                FAILURE - NULL
*****************************************************************/
static void* MemoryPoolAllocFromBlock(mPool_t* pool, mBlock_t* block)
{
    size_t elementSize = sizeof(mUnit_t) + pool->unitSize;
    mUnit_t* unit = (mUnit_t*)(block->units + block->firstAvailable * elementSize);
    block->firstAvailable = unit->nextAvailable;
    block->availableUnitCount -= 1;

    // remove from available block link-list
    if(block->availableUnitCount == 0)
    {
        MemoryPoolRemoveAvailableBlockList(pool, block);
    }

    return unit->data;
}

/*****************************************************************
* Description : Insert a block into pool's total block list
* Parsms      : pool - memory pool
*               block - memory block
* Return      : void
*****************************************************************/
static void MemoryPoolInsertTotalBlockList(mPool_t* pool, mBlock_t* block)
{
    if(pool->firstBlock == NULL)
    {
        pool->firstBlock = block;
        pool->lastBlock = block;
    }
    else
    {
        block->prevBlock = pool->lastBlock;
        pool->lastBlock->nextBlock = block;
        pool->lastBlock = block;
    }
}


/*****************************************************************
* Description : Remove a block from pool's total block list
* Parsms      : pool - memory pool
*               block - memory block
* Return      : void
*****************************************************************/
static void MemoryPoolRemoveTotalBlockList(mPool_t* pool, mBlock_t* block)
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
        pool->lastBlock = block->nextBlock;
    }
    else
    {
        block->nextBlock->prevBlock = block->prevBlock;
    }
}


/*****************************************************************
* Description : Insert a block into pool's available block list
* Parsms      : pool - memory pool
*               block - memory block
* Return      : void
*****************************************************************/
static void MemoryPoolInsertAvailableBlockList(mPool_t* pool, mBlock_t* block)
{
    if(pool->firstAvailableBlock == NULL)
    {
        pool->firstAvailableBlock = block;
        pool->lastAvailableBlock = block;
    }
    else
    {
        block->prevAvailableBlock = pool->lastAvailableBlock;
        pool->lastAvailableBlock->nextAvailableBlock = block;
        pool->lastAvailableBlock = block;
    }
}


/*****************************************************************
* Description : Remove a block from pool's available block list
* Parsms      : pool - memory pool
*               block - memory block
* Return      : void
*****************************************************************/
static void MemoryPoolRemoveAvailableBlockList(mPool_t* pool, mBlock_t* block)
{
    if(block->prevAvailableBlock == NULL)
    {
        pool->firstAvailableBlock = block->nextAvailableBlock;
    }
    else
    {
        block->prevAvailableBlock->nextAvailableBlock = block->nextAvailableBlock;
    }

    if(block->nextAvailableBlock == NULL)
    {
        pool->lastAvailableBlock = block->nextAvailableBlock;
    }
    else
    {
        block->nextAvailableBlock->prevAvailableBlock = block->prevAvailableBlock;
    }
}