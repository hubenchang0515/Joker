# Memory Pool
A thread-safe mempry pool with fixed-size allocation.  

## APIs
```C
const char* MemoryPoolError(mPool_t* pool);
int MemoryPoolCreate(mPool_t* pool, mConfig_t* config);
void* MemoryPoolAlloc(mPool_t* pool, size_t size);
int MemoryPoolFree(mPool_t* pool, void* addr);
void MemoryPoolDestroy(mPool_t* pool);
void MemoryPoolDebug(mPool_t* pool);
```

## Usage
* Create Memory Pool
```C
/* Init a mutex to memory pool */
pthread_mutex_t mutex;
pthread_mutex_init(mutex, NULL);

/* Setting memory pool config struct*/
mConfig_t conf =;
conf.size = 1024; // 1KB per memory unit
conf.count = 32;  // 32 units per memory block
conf.mutex = &mutex;
conf.mutexLock = (int(*)(void*))pthread_mutex_lock;
conf.mutexUnlock = (int(*)(void*))pthread_mutex_unlock;

/* Create memory pool */
mPool_t pool;
MemoryPoolCreate(&pool, &conf);
```

* Alloc Memory Unit
```C
void* ptr = MemoryPoolAlloc(&pool, 1024);
```

* Free Memory
```C
MemoryPoolFree(ptr);
```

* Destroy Memory Pool
```C
MemoryPoolDestroy(&pool);
```

* Get Error Message
```C
printf("%s\n", MemoryPoolError(&pool));
```

* Disply Memory Pool State
```C
MemoryPoolDebug(&pool);
```