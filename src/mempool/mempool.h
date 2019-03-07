#ifndef JOCKER_MEMPOOL_H
#define JOCKER_MEMPOOL_H

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct JockerMemoryPoolStruct mPool_t;
typedef struct JockerMemoryBlockStruct mBlock_t;
typedef struct JockerMemoryUnitStruct mUnit_t;

#ifndef JOCKER_MEMPOOL_ERROR_SIZE
#define JOCKER_MEMPOOL_ERROR_SIZE 128
#endif

/* Struct of memory pool */
struct JockerMemoryPoolStruct
{
    size_t unitSize;                        // size of a unit
    size_t unitCount;                       // unit count of per-block 
    size_t blockCount;                      // block count
    size_t availableBlockCount;             // available block count
    mBlock_t* firstBlock;                   // the first memory block
    mBlock_t* lastBlock;                    // the last memory block
    mBlock_t* firstAvailableBlock;          // the first available memory block
    mBlock_t* lastAvailableBlock;           // the first available memory block
    char error[JOCKER_MEMPOOL_ERROR_SIZE];  // error message
};

/* Struct of memory block */
struct JockerMemoryBlockStruct
{
    size_t firstAvailable;          // the first available memory unit
    size_t lastAvailable;
    size_t availableUnitCount;      // count of available unit  
    mBlock_t* prevBlock;            // the previous memory block
    mBlock_t* nextBlock;            // the next memory block
    mBlock_t* prevAvailableBlock;   // the previous available memory block
    mBlock_t* nextAvailableBlock;   // the next available memory block
    uint8_t units[];                // flexible array of mUnit_t[]
};

/* Struct of memory unit */
struct JockerMemoryUnitStruct
{
    size_t nextAvailable;   // the next available memory unit
    mBlock_t* block;        // belong to which block
    uint8_t data[];         // flexible array
};

#ifdef __cplusplus
    extern "C" {
#endif

const char* MemoryPoolError(mPool_t* pool);
int MemoryPoolCreate(mPool_t* pool, size_t unitSize, size_t unitCount);
void* MemoryPoolAlloc(mPool_t* pool, size_t size);
int MemoryPoolFree(mPool_t* pool, void* addr);
void MemoryPoolDestroy(mPool_t* pool);
void MemoryPoolDebug(mPool_t* pool);

#ifdef __cplusplus
    }
#endif

#endif