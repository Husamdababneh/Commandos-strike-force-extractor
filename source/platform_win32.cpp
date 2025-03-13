/* ========================================================================
   $File: platform_win32.cpp
   $Date: 2025-03-14
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   $Description: 
    ========================================================================*/



// TODO: give me more control on this
// do we want to abstract return value to a struct ?? 
inline void* platform_allocate(size64 size) {
    return VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}

inline void platform_free(void* ptr, size64 size) {
    (void) size;
    VirtualFree(ptr, 0, MEM_RELEASE);
}
