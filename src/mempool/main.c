#include "mempool.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

#define SIZE 1024
#define COUNT 10000
void* pointers[1024] = { NULL };
mPool_t pool;

void systemCall();
void poolCall();

int main()
{
    MemoryPoolCreate(&pool, SIZE, 16);
    systemCall();
    poolCall();

    return 0;
}

void systemCall()
{
    int count = COUNT;
    while(count-- > 0)
    {
        for(size_t i = 0; i < 1024; i++)
        {
            pointers[i] = malloc(SIZE);
        }

        for(size_t i = 0; i < 1024; i++)
        {
            free(pointers[i]);
        }
    }
}

void poolCall()
{
    size_t count = COUNT;
    while(count-- > 0)
    {
        for(size_t i = 0; i < 1024; i++)
        {
            pointers[i] = MemoryPoolAlloc(&pool, SIZE);
        }

        for(size_t i = 0; i < 1024; i++)
        {
            MemoryPoolFree(&pool, pointers[i]);
        }
    }
}