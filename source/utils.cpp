/* ========================================================================
   $File: utils.cpp
   $Date: 2020-03-30
   $Creator: Husam Dababneh
   ========================================================================*/

//#pragma once
#include "utils.h"

#include "zlib.h"
#include "zconf.h"
#include "stdio.h"
#include <sys/stat.h>

// TODO: Remove CRT dependency
#include <string.h>

#define NDEBUG
#define _assert __debugbreak();
#define assert(c) if(c == 0) _assert

#define internal static
#define global   static 

ptr memcpy(ptr dst, const ptr src, u64 cnt)
{
    u8* pszDest = (u8*)dst;
    const u8* pszSource =( const u8*)src;
    if((pszDest!= NULL) && (pszSource!= NULL))
    {
        while(cnt) //till cnt
        {
            //Copy byte by byte
            *(pszDest++)= *(pszSource++);
            --cnt;
        }
    }
    return dst;
}

// TODO: check for aliasing 
// internal
// ptr memcpy(ptr to,
//            const ptr from,
//            u64 numBytes)
// {
//     ptr result = to;
//     u64 counter = 0;
//     while(counter++ != numBytes) ((u8*)to)[counter]=((u8*)from)[counter];
//     return result;
// }

internal
void zerr(int ret)
{
	printf("Unsuccesful decompression : ");
	switch (ret) {
      case Z_ERRNO:{
          if (ferror(stdin))  printf("error reading stdin\n");
          if (ferror(stdout)) printf("error writing stdout\n");
      }break;
          
      case Z_STREAM_ERROR:{
          printf("invalid compression level\n");
      }break;
          
      case Z_DATA_ERROR:{
          printf("invalid or incomplete deflate data\n");
      }break;
          
      case Z_MEM_ERROR:{
          printf("out of memory\n");
      }break;
          
      case Z_VERSION_ERROR:{
          printf("zlib version mismatch!\n");
      }break;
         
	}
	printf("ret : %d", ret);
    printf("Error While Decompression");
	assert(false);
}

internal
void decompress(char* source,
                int sourceSize,
                char* dest,
                int destSize)
{
	
	z_stream infstream;
	infstream.zalloc = Z_NULL;
	infstream.zfree = Z_NULL;
	infstream.opaque = Z_NULL;
	
	infstream.avail_in = (unsigned int)sourceSize; // size of input
	infstream.next_in = (Bytef*)source; // input char array
	
	infstream.avail_out = (unsigned int)destSize; // size of output
	infstream.next_out = (Bytef*)dest; // output char array
	
	// TODO(husam): handle errors comeing from  (inflate)
	int ret = inflateInit(&infstream);
	if(ret < Z_OK) zerr(ret);
	ret = inflate(&infstream, Z_NO_FLUSH);
	if(ret < Z_OK ) zerr(ret);
	inflateEnd(&infstream);
	
}

internal
u64 read_entire_file(FILE* file, void** data_return)
{
	assert(file);
	int descriptor = fileno(file);
	
	struct stat file_stats;
	int result = fstat(descriptor, &file_stats);
	if (result == -1) return -1;
	
	u64 length = file_stats.st_size;
	
	unsigned char* data = new unsigned char[length];
	
	fseek(file, 0, SEEK_SET);
	u64 success = fread((void*)data, length, 1, file);
	if (success < 1) {
		delete[] data;
		return -1;
	}
	
	*data_return = data;
	return length;
}

internal
u64 read_entire_file(const char* filepath, void** data_return)
{
    FILE* handle;
    fopen_s(&handle, filepath, "rb");
	if (!handle)
	{
        // THIS API IS SHIT, If the file is already in use it wont tell you
		printf("Couldn't find file [%s]\n", filepath);
		return false;
	}
	
	u64 result = read_entire_file(handle, data_return);
	fclose(handle);
	return result;
}
/*
void insure_dirs_exists(u8* filepath)
{
	u32 directories_count = 0;
	u8 filepath_b[512];
	memcpy(filepath_b, filepath, strlen(filepath));
    
	u8* cur = filepath;
	while(*cur++ != '\0'){
		if (*cur == '/') directories_count++;
	}
    
	if(directories_count == 0) return;
	
	// final path
	char*  next_token = nullptr;
    
	u8  dir[512] = {0};
	u32 size     = 0;
	u32 offset   = 0;
	u8* token    = strtok(filepath_b, "/");
	for(u32 i = 0; i < directories_count; i++)
	{
		size = strlen((char*)token);
		strcat_s(dir, strlen(dir) + size + 1, token);
		dir[strlen(dir)] = '/';
		CreateDirectoryA(dir, nullptr);
		token = strtok(NULL, "/", &next_token);
	}

}
*/
#define OPEN_FILE(handle, path, mode) FILE* handle; fopen_s(&handle, path, mode);




// TODO: add this to "HD Base"
const u8* operator ""_u8(const char* str, size_t size)
{
    return (const u8*)str;
}

SV operator ""_sv(const char* str, size_t size)
{
    return {size, (const u8*)str};
}

static
u64 strlen(u8* str)
{
    u8* it = str;
    while(*++it != NULL);
    return it - str;
}

static
SV to_sv(const char* str)
{
    return SV{strlen(str), (u8*)str};
}


static
SV to_sv(u8* str)
{
    return {strlen(str), str};
}


static
u64 indexOf(const SV str, u8 dil)
{
    const u8* it = str.str;
    const u8* end = str.str + str.size;
    while(*++it != dil && it != end);
    return it - str.str;
}

static
u64 indexOf_r(const SV str, u8 dil)
{
    const u8* start = str.str;
    const u8* it = str.str + str.size;
    while(*--it != dil && it != start);
    return it - start;
}


static
SV strtok(SV tok, u8 dil, SV* remainder)
{
    const u8* it = tok.str;
    const u8* end = tok.str + tok.size;
    while(it++ != end)
    {
        if (*it == dil)
        {
            u64 size = (u64)(it - tok.str);
            remainder->str = tok.str + size;
            remainder->size = tok.size - size;
            return {size, tok.str};
        }
    }
   
    remainder->str = NULL;
    remainder->size = 0;
    return tok;
}


// maybe you want to start with a simple u8*
static
SV strtok(const u8* str, u8 dil)
{
    const u8* it = str;
    while(*++it != dil);
    // @Incomplete: what happens if the dil is the last char
    // We can safely cast to unsigned because we always subtract the end from the start
    return {(u64)(it - str), str};
}



void win32_print_error()
{
    DWORD errorCode = GetLastError();
    if (errorCode != 183)
    {
        DWORD errorCode = GetLastError();
        printf("Could not create folder\n");
        printf("Error Code: %d\n", errorCode);
        exit(-1);
    }
}


static
File createFile(const SV filePath, u64 flags = CREATE_ALWAYS, u64 access = GENERIC_READ | GENERIC_WRITE)
{
    // @Incomplete: Assert here
    //assert(filePath.size < 255);
    File file = {};
    
    // Insure that the path is valid
    const u8* it = filePath.str;
    u64 dirCounter = 0;
    while(*it++ != NULL) {
        if (*it == '\\' || *it == '/') dirCounter++;
    }

    if (dirCounter != 0)
    {
        // TODO: Create dirs here

        u64 count       = 0;
        u8  buffer[256] = {};

        u64 index = indexOf_r(filePath, '/');
        SV dirsPath = {index, filePath.str};
        SV remainder;
        SV token = strtok(dirsPath, '/', &remainder);
        memcpy(buffer + count, token.str, token.size);
        count = count + token.size;
        if (CreateDirectoryA((LPCSTR)buffer, NULL) == 0)
        {
            // TODO: Get Last Error
            win32_print_error();
        }
        u64 counter = 1;
        //printf("%lld: %.*s\n", counter, SV_PRINT(token));
        while(remainder.size != 0)
        {
            token = strtok(remainder, '/', &remainder);
            memcpy(buffer + count, token.str, token.size);
            count = count + token.size;
            // printf("%lld: %.*s\n", counter, SV_PRINT(token));
            if (CreateDirectoryA((LPCSTR)buffer, NULL) == 0)
            {
                win32_print_error();          
            }
            counter++;
         }
    }


    


    HANDLE fileHandle =  CreateFileA(
        (LPSTR)filePath.str,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
        );
    

    file.win32_handle = fileHandle;
    
    return file;
}


b8 writeFile(File file, ptr buffer, u64 nNumberOfBytesToWrite, u64* lpNumberOfBytesWritten)
{
    u8 result = (u8)WriteFile(file.win32_handle,
                              buffer,
                              nNumberOfBytesToWrite,
                              (LPDWORD)lpNumberOfBytesWritten,
                              NULL);
    return result;
};


b8 closeFile(File file)
{
    //TODO: if windows
    if (CloseHandle(file.win32_handle) != 0)
    {
        // Get last error
        return false;
    }
    return true;
}


//-

#define KiloByte(x) x_KB;
#define MegaByte(x) x_MB;

static constexpr
u64 operator ""_KB(u64 kiloBytes) {
    return kiloBytes * 1024;
}

static constexpr
u64 operator ""_MB(u64 megaBytes) {
    return megaBytes * 1024 * 1012;
}
