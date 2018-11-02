# Coroutine
Coroutine by switch-case.

## API
```C
/* Create Coroutine */
void CoCreate(Coroutine* output, CoFunc func);

/* Resume Coroutine */
void* CoResume(Coroutine* input, void* userdate);

/* Tield Cotoutine */
void CoYield(co, value);

/* Used in coroutine function */
void CoBegin(Coroutine* input);
void CoEnd();
``` 
* Local variable in function of coroutine must have to be ``static``.
* Cannot create more than one cotoutines by same function.

## Demo
```C
#include <stdio.h>
#include "coroutine.h"

void* func(Coroutine* co, void* data);

#define R_EXIT   (void*)0
#define R_YIELD  (void*)1

int main()
{
	Coroutine co;
	CoCreate(&co, func);

	while(R_EXIT != CoResume(&co, "hello world"));
	
}


void* func(Coroutine* co, void* data)
{
	// must have to use static variable in coroutine
	static int i = 0; 
	
	CoBegin(co);
	
	for(; i < 10; i++)
	{
		printf("%s %d\n", data, i);
		CoYield(co, R_YIELD);
	}
	
	CoEnd();
	
	return R_EXIT;
}
```