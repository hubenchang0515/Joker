# Leak Check
A simple memory leak analyzer.

# Usage Demo
```C
#include "leakcheck.h"

void func1()
{
	int* p = malloc(32);
	free(p);
}

void func2()
{
	int* p = malloc(64); // leak
}

void func3()
{
	int* p = malloc(128);
	free(p);
}

void func4()
{
	int* p = malloc(256); // leak
}

void func5()
{
	int* p = malloc(512);
	free(p);
}

int main()
{
	func1();
	func2();
	func3();
	func4();
	func5();
	lc_result();
	return 0;
}
```

Run
```C
main.c func4 22 : 256 bytes
main.c func2 11 : 64 bytes
total : 2 times 320 bytes.
```