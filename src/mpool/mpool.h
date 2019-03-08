#ifndef JOCKER_MPOOL_H
#define JOCKER_MPOOL_H

#include <stdint.h>
#include <stddef.h>


typedef struct MemoryPool m_pool_t;
typedef struct MemoryUnit m_unit_t;

struct MemoryPool
{
    m_unit_t* first;
    size_t totalSize;
    size_t unitSize;
};

struct MemoryUnit
{
    m_unit_t* next;
    uint8_t data[];
};

#ifndef MPOOl_SIZE
#define MPOOL_SIZE(byte, count) ((byte + sizeof(m_unit_t)) * count)
#endif

#ifdef __cplusplus
    extern "C" {
#endif

int mpool_init(m_pool_t* pool, void* memory, size_t bytes, size_t unitBytes);
void* mpool_malloc(m_pool_t* pool);
void mpool_free(m_pool_t* pool, void* addr);
void mpool_debug(m_pool_t* pool);

#ifdef __cplusplus
    }
#endif

#endif