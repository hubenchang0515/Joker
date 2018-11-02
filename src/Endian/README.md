# Endian
Write or Read data in Big-Endian or Little-Endian whatever you want.

## API
```C
/* Big-Endian */
uint16_t readUint16InBigEndian(void* memory);
uint32_t readUint32InBigEndian(void* memory);
uint64_t readUint64InBigEndian(void* memory);

void writeUint16InBigEndian(void* memory, uint16_t value);
void writeUint32InBigEndian(void* memory, uint32_t value);
void writeUint64InBigEndian(void* memory, uint64_t value);

int16_t readInt16InBigEndian(void* memory);
int32_t readInt32InBigEndian(void* memory);
int64_t readInt64InBigEndian(void* memory);

void writeInt16InBigEndian(void* memory, int16_t value);
void writeInt32InBigEndian(void* memory, int32_t value);
void writeInt64InBigEndian(void* memory, int64_t value);


/* Little-Endian */
uint16_t readUint16InLittleEndian(void* memory);
uint32_t readUint32InLittleEndian(void* memory);
uint64_t readUint64InLittleEndian(void* memory);

void writeUint16InLittleEndian(void* memory, uint16_t value);
void writeUint32InLittleEndian(void* memory, uint32_t value);
void writeUint64InLittleEndian(void* memory, uint64_t value);

int16_t readInt16InLittleEndian(void* memory);
int32_t readInt32InLittleEndian(void* memory);
int64_t readInt64InLittleEndian(void* memory);

void writeInt16InLittleEndian(void* memory, int16_t value);
void writeInt32InLittleEndian(void* memory, int32_t value);
void writeInt64InLittleEndian(void* memory, int64_t value);
```

## Demo
```C
#include <stdio.h>
#include "endian.h"

int main()
{
	char memory[8];
	
	writeUint16InBigEndian(memory,UINT16_MAX);
	printf("%u\n",readUint16InBigEndian(memory));
	writeUint32InBigEndian(memory,UINT32_MAX);
	printf("%u\n",readUint32InBigEndian(memory));
	writeUint64InBigEndian(memory,UINT64_MAX);
	printf("%lu\n",readUint64InBigEndian(memory));
	
	writeInt16InBigEndian(memory,INT16_MIN);
	printf("%d\n",readInt16InBigEndian(memory));
	writeInt32InBigEndian(memory,INT32_MIN);
	printf("%d\n",readInt32InBigEndian(memory));
	writeInt64InBigEndian(memory,INT64_MIN);
	printf("%ld\n",readInt64InBigEndian(memory));
	
	writeUint16InLittleEndian(memory,UINT16_MAX);
	printf("%u\n",readUint16InLittleEndian(memory));
	writeUint32InLittleEndian(memory,UINT32_MAX);
	printf("%u\n",readUint32InLittleEndian(memory));
	writeUint64InLittleEndian(memory,UINT64_MAX);
	printf("%lu\n",readUint64InLittleEndian(memory));
	
	writeInt16InLittleEndian(memory,INT16_MIN);
	printf("%d\n",readInt16InLittleEndian(memory));
	writeInt32InLittleEndian(memory,INT32_MIN);
	printf("%d\n",readInt32InLittleEndian(memory));
	writeInt64InLittleEndian(memory,INT64_MIN);
	printf("%ld\n",readInt64InLittleEndian(memory));
	
	return 0;
}
```