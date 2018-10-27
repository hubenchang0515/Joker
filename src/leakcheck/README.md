# Leak Check
A simple memory leak analyzer.

# Usage Demo
```C
#include "leakcheck.h"
#include <time.h>

typedef void* ptr_t;

ptr_t p1[10];
ptr_t p2[10];

void test_malloc1(size_t index, size_t bytes)
{
	p1[index] = malloc(bytes);
}

void test_malloc2(size_t index, size_t bytes)
{
	p2[index] = malloc(bytes);
}

void test_free1(size_t index)
{
	free(p1[index]);
}

void test_free2(size_t index)
{
	free(p2[index]);
}



int main()
{
	/* 分配内存 */
	for(size_t i = 0; i < 10; i++)
	{
		test_malloc1(i,100*i + 100);
		test_malloc2(i,200*i + 200);
	}
	
	/* 随机释放部分内存 */
	srand(time(NULL));
	for(size_t i = 0; i < 7; i++)
	{
		test_free1(rand() % 10);
		test_free2(rand() % 10);
	}
	
	lc_detail();
	lc_statistic();
}
```

Rusult
```C
[Details]
In file <main.c> line <16> function <test_malloc2> : leak 2000 bytes
In file <main.c> line <11> function <test_malloc1> : leak 900 bytes
In file <main.c> line <16> function <test_malloc2> : leak 1600 bytes
In file <main.c> line <11> function <test_malloc1> : leak 800 bytes
In file <main.c> line <11> function <test_malloc1> : leak 700 bytes
In file <main.c> line <16> function <test_malloc2> : leak 1200 bytes
In file <main.c> line <11> function <test_malloc1> : leak 600 bytes
In file <main.c> line <11> function <test_malloc1> : leak 500 bytes
In file <main.c> line <16> function <test_malloc2> : leak 600 bytes
In file <main.c> line <16> function <test_malloc2> : leak 200 bytes
Total : 10 times 9100 bytes.

[Statistics]
In file <main.c> line <16> : leak 5 times 5600 bytes
In file <main.c> line <11> : leak 5 times 3500 bytes
Total : 10 times 9100 bytes.

```