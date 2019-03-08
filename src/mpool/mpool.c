#include "mpool.h"
#include <stdio.h>
int mpool_init(m_pool_t* pool, void* memory, size_t bytes, size_t unitBytes)
{
    uint8_t* limit = (uint8_t*)memory + bytes;
    size_t unitSize = sizeof(m_unit_t) + unitBytes;

    uint8_t* ptr = memory;
    pool->first = memory;
    for(; ptr + unitSize < limit; ptr += unitSize)
    {
        ((m_unit_t*)ptr)->next = (m_unit_t*)(ptr + unitSize);
    }
    ((m_unit_t*)ptr)->next = NULL;
}

void* mpool_malloc(m_pool_t* pool)
{
    if(pool->first == NULL)
    {
        return NULL;
    }

    m_unit_t* unit = pool->first;
    pool->first = pool->first->next;

    return unit->data;
}

void mpool_free(m_pool_t* pool, void* addr)
{
    m_unit_t* unit = (m_unit_t*)((uint8_t*)addr - sizeof(m_unit_t));
    unit->next = pool->first;
    pool->first = unit;
}

void mpool_debug(m_pool_t* pool)
{
    printf("\nfirst -> %p\n", pool->first);
    for(m_unit_t* ptr = pool->first; ptr != NULL; ptr = ptr->next)
    {
        printf("%p -> %p\n", ptr, ptr->next);
    }
}