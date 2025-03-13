/* ========================================================================
   $File: base_platform.cpp
   $Date: 2025-03-14
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   $Description: 
    ========================================================================*/


#include "base_platform.h"

void* allocate_memory(size64 size) {
    return platform_allocate(size);
}

void free_memory(void* ptr, size64 size) {
    platform_free(ptr, size);
}
