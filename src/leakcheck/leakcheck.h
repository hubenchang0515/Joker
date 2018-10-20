#ifndef LEAKCHECK_H
#define LEAKCHECK_H

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct lc_node_t lc_node_t;

extern lc_node_t* lc_list_head;

void* lc_malloc(size_t bytes, const char* file, const char* function, size_t line);
int lc_free(void* ptr);
void lc_result(void);

#ifdef malloc
	#undef malloc
#endif
#define malloc(bytes) lc_malloc(bytes, __FILE__, __FUNCTION__, __LINE__)

#ifdef free
	#undef free
#endif
#define free(ptr) lc_free(ptr)

#endif