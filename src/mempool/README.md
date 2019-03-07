# Memory Pool
A mempry pool with fixed-size allocation.  

## APIs
```C
const char* MemoryPoolError(mPool_t* pool);
int MemoryPoolCreate(mPool_t* pool, size_t unitSize, size_t unitCount);
void* MemoryPoolAlloc(mPool_t* pool, size_t size);
int MemoryPoolFree(mPool_t* pool, void* addr);
void MemoryPoolDestroy(mPool_t* pool);
void MemoryPoolDebug(mPool_t* pool);
```

## Usage Demo
```C
#include "mempool.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

#define SIZE 128
#define COUNT 100000
#define ARRAY 1024
void* pointers[ARRAY] = { NULL };
mPool_t pool;

void systemCall();
void poolCall();

int main()
{
    MemoryPoolCreate(&pool, SIZE, 32);
    poolCall();
    systemCall();
    return 0;
}

void systemCall()
{
    int count = COUNT;
    size_t start = clock();
    while(count-- > 0)
    {
        for(size_t i = 0; i < ARRAY; i++)
        {
            pointers[i] = malloc(SIZE);
        }

        for(size_t i = 0; i < ARRAY; i++)
        {
            free(pointers[i]);
        }
    }
    size_t end = clock();
    printf("malloc/free use %d ms\n", end - start);
}

void poolCall()
{
    size_t count = COUNT;
    size_t start = clock();
    while(count-- > 0)
    {
        for(size_t i = 0; i < ARRAY; i++)
        {
            pointers[i] = MemoryPoolAlloc(&pool, SIZE);
        }

        for(size_t i = 0; i < ARRAY; i++)
        {
            MemoryPoolFree(&pool, pointers[i]);
        }
    }
    size_t end = clock();
    printf("memory pool use %d ms\n", end - start);
}
```