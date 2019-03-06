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
    size_t unitSize;       // size of a unit
    size_t unitCount;      // unit count of per-block 
    mBlock_t* firstBlock;  // the first memory block
    mBlock_t* lastBlock;  // the last memory block
    char error[JOCKER_MEMPOOL_ERROR_SIZE];   // error message
};

/* Struct of memory block */
struct JockerMemoryBlockStruct
{
    size_t firstUnit;    // the first available memory unit
    size_t available;    // count of available unit  
    mBlock_t* prevBlock; // the previous memory block
    mBlock_t* nextBlock; // the next memory block
    uint8_t datas[];     // flexible array [index][data]
};

/* Struct of memory unit */
struct JockerMemoryUnitStruct
{
    size_t nextUnit; // the next available memory unit
    uint8_t data[];  // flexible array
};

#ifdef __cplusplus
    extern "C" {
#endif

const char* MemoryPoolError(mPool_t* pool);
int MemoryPoolCreate(mPool_t* pool, size_t unitSize, size_t unitCount);
void* MemoryPoolAlloc(mPool_t* pool, size_t size);
int MemoryPoolFree(mPool_t* pool, void* addr);
void MemoryPoolDestroy(mPool_t* pool);

#ifdef __cplusplus
    }
#endif

#endif