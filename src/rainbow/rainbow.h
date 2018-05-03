#ifndef RAINBOW_H
#define RAINBOW_H

#ifdef __cplusplus
    extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>

#ifdef __linux__

typedef enum ForegroundColor
{
    fgCURRENT,
    fgBLACK = 30,
    fgRED,
    fgGREEN,
    fgYELLOW,
    fgBLUE,
    fgPURPLE,
    fgLIGHTBLUE,
    fgWHITE
}FgColor; 
 
typedef enum BackgroundColor
{
    bgCURRENT,
    bgBLACK = 40,
    bgRED,
    bdGREEN,
    bgYELLOW,
    bgBLUE,
    bgPURPLE,
    bgLIGHTBLUE,
    bgWHITE
}BgColor; 
 
 
/*
 * Param
 *  fp          : stdout or stderr
 *  fgColor     : foreground color
 *  bgColor     : background color
 *  fmt         : format string , same to printf
 *  ...         : more params , same to printf
 *
 */
static int rb_printf(FILE* fp,FgColor fgcolor,BgColor bgcolor,const char* fmt,...)
{
    if(bgcolor < bgBLACK || bgcolor > bgWHITE)
    {
        fprintf(fp,"\033[");
    }
    else
    {
        fprintf(fp,"\033[%2d;",bgcolor);
    }
  
    if(fgcolor >= fgBLACK && fgcolor <= fgWHITE)
    {
        fprintf(fp,"%d;",fgcolor);
    }
     
    fprintf(fp,"1m");
     
    va_list args;
    va_start(args,fmt);
    int rval = vfprintf(fp,fmt,args);
    va_end(args);
     
    fprintf(fp,"\033[0m");
     
    return rval;
}

#endif


#ifdef _WIN32

#include <windows.h>

typedef enum ForegroundColor
{
    fgCURRENT = 0xff,
    fgBLACK = 0,
    fgBLUE,
    fgGREEN,
    fgLIGHTBLUE,
    fgRED,
    fgPURPLE,
    fgYELLOW,
    fgWHITE
}FgColor; 
 
typedef enum BackgroundColor
{
    bgCURRENT = 0xff,
    bgBLACK = 0,
    bgBLUE,
    bgGREEN,
    bgLIGHTBLUE,
    bgRED,
    bgPURPLE,
    bgYELLOW,
    bgWHITE
}BgColor; 


/*
 * Param
 *  fp          : stdout or stderr
 *  fgColor     : foreground color
 *  bgColor     : background color
 *  fmt         : format string , same to printf
 *  ...         : more params , same to printf
 *
 */
static int rb_printf(FILE* fp, FgColor fgcolor,BgColor bgcolor,const char* fmt,...)
{
	HANDLE handle;
	if(fp == stdout)
	{
		handle = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	else if(fp == stderr)
	{
		handle = GetStdHandle(STD_ERROR_HANDLE);
	}
	else
	{
		return -1;
	}
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(handle, &info);
	DWORD fg = fgcolor != fgCURRENT ? fgcolor | FOREGROUND_INTENSITY : info.wAttributes & 0x0f;
	DWORD bg = bgcolor != bgCURRENT ? (bgcolor << 4) | BACKGROUND_INTENSITY : info.wAttributes & 0xf0;
	SetConsoleTextAttribute(handle, fg | bg );
	
	va_list args;
    va_start(args,fmt);
    int rval = vfprintf(fp,fmt,args);
    va_end(args);
	SetConsoleTextAttribute(handle, info.wAttributes);
	
	return rval;
}

#endif

#ifdef __cplusplus
    }
#endif

#endif // RAINBOW_H