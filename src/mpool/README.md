# MPool
Very easy memory pool with fixed-size allocation

## API
```C
#define MPOOL_SIZE(byte, count)

int mpool_init(pool_t* pool, void* memory, size_t bytes, size_t unitBytes);
void* mpool_malloc(pool_t* pool);
void mpool_free(pool_t* pool, void* addr);
```

## Usage
```C
#include "mpool.h"
#include <stdio.h>
#include <stdlib.h>

#define COUNT 10

int main()
{
    m_pool_t pool;
    void* buff = malloc(MPOOL_SIZE(1024, COUNT));
    mpool_init(&pool, buff, MPOOL_SIZE(1024, COUNT), 1024);
    void* pointers[COUNT] = {NULL};

    mpool_debug(&pool);

    for(int i = 0; i < COUNT; i++)
    {
        pointers[i] = mpool_malloc(&pool);
    }

    mpool_debug(&pool);

    for(int i = 0; i < COUNT; i++)
    {
        mpool_free(&pool, pointers[i]);
    }

    mpool_debug(&pool);

    for(int i = 0; i < COUNT; i++)
    {
        pointers[i] = mpool_malloc(&pool);
    }

    mpool_debug(&pool);

}

```