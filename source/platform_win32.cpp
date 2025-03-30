/* ========================================================================
   $File: platform_win32.cpp
   $Date: 2025-03-14
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   $Description: 
    ========================================================================*/




// TODO: give me more control on this
// do we want to abstract return value to a struct ??

// [[deprecated]]
proc inline
void* platform_allocate_memory(const size64 size) {
    return VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}

proc inline
void platform_free_memory(void* ptr, const size64 size) {
    (void) size;
    VirtualFree(ptr, 0, MEM_RELEASE);
}


proc inline
vptr platform_reserve_memory(const size64 size) {
    return VirtualAlloc(NULL, size, MEM_RESERVE, PAGE_READWRITE);
}

proc inline
void platform_commit_memory(vptr ptr, size64 size) {
    VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE);
}


proc 
SystemInfo platform_get_system_info() {
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);

    SystemInfo result;

    switch(systemInfo.wProcessorArchitecture) {
      case PROCESSOR_ARCHITECTURE_AMD64:   {result.processorArchitecture = AMD64;  }break;
      case PROCESSOR_ARCHITECTURE_ARM:     {result.processorArchitecture = ARM;    }break;
      case PROCESSOR_ARCHITECTURE_ARM64:   {result.processorArchitecture = ARM64;  }break;
      case PROCESSOR_ARCHITECTURE_IA64:    {result.processorArchitecture = IA64;   }break;
      case PROCESSOR_ARCHITECTURE_INTEL:   {result.processorArchitecture = X86;    }break;
      case PROCESSOR_ARCHITECTURE_UNKNOWN: {result.processorArchitecture = Unknown;}break;
    }
    result.pageSize         = systemInfo.dwPageSize;
    result.granularity      = systemInfo.dwAllocationGranularity;
    // result.activeProcessors = systemInfo.dwActiveProcessorMask;
    result.minAppAddress    = systemInfo.lpMinimumApplicationAddress;
    result.maxAppAddress    = systemInfo.lpMaximumApplicationAddress;
    
    return result;
}
