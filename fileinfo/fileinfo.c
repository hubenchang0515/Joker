#include "fileinfo.h"
#include <stddef.h>

/********************************************************************
 * Get the directory from it's path 
 * Input  : /home/hubenchang0515/Joker/Joker.tar.gz
 * Output : Joker.tar.gz
*********************************************************************/
const char* fileinfo_name(const char* path, char* output, size_t size)
{
    const char* p = path + strlen(path) - 1;
	for(; p!= path && *p != '/' && *p != '\\'; p--);
	
	p = p == path ? path : p + 1;
	strncpy(output, p, size-1);
	output[size-1] = 0;

	return output;
}

/********************************************************************
 * Get the directory from it's path 
 * Input  : /home/hubenchang0515/Joker/Joker.tar.gz
 * Output : /home/hubenchang0515/Joker
*********************************************************************/
const char* fileinfo_directory(const char* path, char* output, size_t size)
{
	const char* p = path + strlen(path) - 1;
	size_t length = strlen(path);
	for(; p!= path && *p != '/' && *p != '\\'; p-- , length--);
	length--;
	
	if(length < size)
	{
		strncpy(output, path, length);
		output[length] = 0;
	}

	return output;
}


/********************************************************************
 * Get the full extension name from it's path 
 * Input  : /home/hubenchang0515/Joker/Joker.tar.gz
 * Output : tar.gz
*********************************************************************/
const char* fileinfo_fullExtension(const char* path, char* output, size_t size)
{
	const char* p = path + strlen(path) - 1;
	const char* dot = NULL;
	for(; p!= path && *p != '/' && *p != '\\'; p--)
	{
		if(*p == '.')
		{
			dot = p + 1;
		}
	}

	if(dot == NULL)
	{
		return NULL;
	}
	else
	{
		strncpy(output, dot, size-1);
		output[size-1] = 0;

		return output;
	}
}



/********************************************************************
 * Get the last extension name from it's path 
 * Input  : /home/hubenchang0515/Joker/Joker.tar.gz
 * Output : gz
*********************************************************************/
const char* fileinfo_lastExtension(const char* path, char* output, size_t size)
{
	const char* p = path + strlen(path) - 1;
	const char* dot = NULL;
	for(; p!= path && *p != '/' && *p != '\\'; p--)
	{
		if(*p == '.')
		{
			dot = p + 1;
			break;
		}
	}

	if(dot == NULL)
	{
		return NULL;
	}
	else
	{
		strncpy(output, dot, size-1);
		output[size-1] = 0;

		return output;
	}
}