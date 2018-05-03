# Joker
Topsy-turvy codes I writing in normal times.
  
# Summary
For example, I need to get file name from it's full path today.  So I write this code.
```C
const char* filename(const char* filepath)
{
	const char* p = filepath + strlen(filepath) - 1;
	for(; p!= filepath && *p != '/' && *p != '\\'; p--);
	
	if(p == filepath)
		return p;
	else
		return p + 1;
}
```

After a month, I need to get file name from it's full path again. So I write this code again.  
After a month again, I write this code again.  
After a month again, I write this code again.  
Again agian again.  

Now I save it at here to let me can reuse it no matter when and where.  

## Use this whole library
Functions of the overwhelming majority may not be used. So this way is not recommended.  

Download this repository and build a library(.lib .dll .a .so).   
Include the header file and link the library.  

```Shell
make
sudo make install
```

## Use the the function you need
Find the function by the [Index](#index) , and copy the code.  

# To Do
 - [x] File info dealing  ([src/fileinfo](src/fileinfo))
 - [x] Coroutine ([src/coroutine](src/coroutine))
 - [x] Print colorful charachters in console ([src/rainbow](src/rainbow))


# Index
