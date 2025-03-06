/* ========================================================================
   $File: utils.h
   $Date: 2023-07-02
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   $Description: 
    ========================================================================*/
#pragma once

#include <stdint.h>
#include <windows.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t b8;

typedef float  f32;
typedef double  f64;

typedef float  float32;
typedef double float64;

typedef void*  ptr;


u64 strlen(const u8* str);
ptr memcpy(ptr to, const ptr from, size_t numBytes);
u8* strtok(char *str, const u8* delim);

struct File
{
#if defined(_WIN64)
    HANDLE win32_handle;
#endif
};

#define SIZED_STRING "%.*s"
#define SV_PRINT(x) (int)x.size, x.str
struct StringView
{
    u64 size;
    const u8* str;
};

typedef StringView SV;
