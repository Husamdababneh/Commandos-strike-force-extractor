/* ========================================================================
   $File: base_platform.cpp
   $Date: 2025-03-14
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   $Description: 
   ========================================================================*/


#include "base_platform.h"

proc inline
vptr allocate_memory(const size64 size) {
    return platform_allocate_memory(size);
}

proc inline
void free_memory(void* ptr, const size64 size) {
    platform_free_memory(ptr, size);
}

proc inline
vptr reserve_memory(const size64 size) {
    return platform_reserve_memory(size);
}

proc inline
void commit_memory(const vptr ptr, const size64 size) {
    return platform_commit_memory(ptr, size);
}

proc
SystemInfo getSystemInfo() {
    // TODO: Make this Global variable, no need to fetch this data everytime we want to get the info
    return platform_get_system_info();
}


// This should go to platform_<platform>.cpp files
extern "C" {
// proc inline
int _fltused;
#pragma function(memset)
vptr memset(vptr dest, s32 c, size64 count)
{
    char *bytes = (char *)dest;
    while (count--)
    {
        *bytes++ = (char)c;
    }
    return dest;
}
#pragma function(memcpy)
vptr memcpy(vptr dest, vcptr src, size64 count) {
    char *dest8 = (char *)dest;
    const char *src8 = (const char *)src;
    while (count--)
    {
        *dest8++ = *src8++;
    }
    return dest;
}
}
