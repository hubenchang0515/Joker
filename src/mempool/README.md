# Memory Pool
A mempry pool with fixed-size allocation.  
`Slower than malloc/free`

## APIs
```C
const char* MemoryPoolError(mPool_t* pool);
int MemoryPoolCreate(mPool_t* pool, size_t unitSize, size_t unitCount);
void* MemoryPoolAlloc(mPool_t* pool, size_t size);
int MemoryPoolFree(mPool_t* pool, void* addr);
void MemoryPoolDestroy(mPool_t* pool);
```

## Usage Demo
```C
#include "mempool.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

int main()
{
    mPool_t pool;
    MemoryPoolCreate(&pool, 2048, 32);
    void* pointers[1024] = { NULL };

    int count = 1000;
    clock_t start = clock();
    while(count-- > 0)
    {
        for(size_t i = 0; i < 1024; i++)
        {
            pointers[i] = malloc(2048);
        }

        for(size_t i = 0; i < 1024; i++)
        {
            free(pointers[i]);
        }
    }
    clock_t end = clock();
    printf("malloc/free use time : %d ms\n", end - start);

    count = 1000;
    start = clock();
    while(count-- > 0)
    {
        for(size_t i = 0; i < 1024; i++)
        {
            pointers[i] = MemoryPoolAlloc(&pool, 2048);
        }

        for(size_t i = 0; i < 1024; i++)
        {
            MemoryPoolFree(&pool, pointers[i]);
        }
    }
    end = clock();
    printf("Memory pool use time : %d ms\n", end - start);

    return 0;
}
```