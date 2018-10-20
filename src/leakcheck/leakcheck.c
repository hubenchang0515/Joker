#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

/* 内存分配记录表节点 */
typedef struct lc_node_t lc_node_t;
struct lc_node_t
{
	void* ptr;
	size_t bytes;
	const char* file;
	const char* function;
	size_t line;
	lc_node_t* next;
};

lc_node_t* lc_list_head = NULL; // 头指针


/* 分配内存，并记录到链表中 */
void* lc_malloc(size_t bytes, const char* file, const char* function, size_t line)
{
	/* 创建一个节点 */
	lc_node_t* node = malloc(sizeof(lc_node_t));
	if(node == NULL)
		return NULL;
	
	/* 分配内存 */
	node->ptr = malloc(bytes);
	if(node->ptr == NULL)
	{
		free(node);
		return NULL;
	}
	
	/* 记录内存分配的信息 */
	node->bytes = bytes;
	node->file = file;
	node->function = function;
	node->line = line;
	
	/* 插入到链表开头 */
	node->next = lc_list_head;
	lc_list_head = node;
	
	return node->ptr;
}


/* 释放内存，并清理链表 */
int lc_free(void* ptr)
{
	lc_node_t* prev = NULL;
	lc_node_t* curr = lc_list_head;
	
	/* 从链表中查找对应的节点 */
	for(; curr != NULL; )
	{
		if(curr->ptr == ptr)
		{
			break;
		}
		prev = curr;
		curr = curr->next;
	}
	
	/* 清除对应的节点并释放内存 */
	if(curr != NULL)
	{
		if(prev != NULL)
		{
			prev->next = curr->next;
		}
		
		if(curr == lc_list_head)
		{
			lc_list_head = curr->next;
		}
		
		free(curr);
		free(ptr);
		
		return 0;
	}
	else
	{
		return 1;
	}
}


/* 打印未释放的内存信息 */
void lc_result(void)
{
	lc_node_t* curr = lc_list_head;
	size_t times = 0;
	size_t bytes = 0;
	/* 遍历打印链表 */
	for(; curr != NULL; curr = curr->next)
	{
		
		//printf("%s\n", curr->function);
		printf("%s %s %lu : %lu bytes\n", curr->file, curr->function, curr->line, curr->bytes);
		times += 1;
		bytes += curr->bytes;
	}
	printf("total : %lu times %lu bytes.\n", times, bytes);
}