/* ========================================================================
   $File: platform.h
   $Date: 2025-03-14
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   $Description: 
    ========================================================================*/

#ifndef PLATFORM_H
#define PLATFORM_H

#include "base.h" 

#if OS_WINDOWS
    // Put these inside if #if to give me the ability to enable/disable these at will 
    #define WIN32_LEAN_AND_MEAN  // Exclude rarely-used Windows features
    #define NOMINMAX             // Prevent definition of min and max macros
    #define NOSERVICE            // Exclude Windows Service APIs
    #define NOHELP               // Exclude Help engine APIs
    #define NOGDI                // Exclude Graphics Device Interface (GDI) APIs
    #define NOKERNEL             // Exclude KERNEL APIs
    #define NOUSER               // Exclude USER (UI-related) APIs
    #define NOSYSPARAMSINFO      // Exclude SystemParametersInfo API
    #define NOMCX                // Exclude modem control API
    #define NOIME                // Exclude Input Method Editor (IME) APIs
    #define NORASTEROPS          // Exclude raster operation definitions
    #define NOMETAFILE           // Exclude metafile APIs
    #define NOWINMESSAGES        // Exclude message-related APIs
    #define NOWINSTYLES          // Exclude window style definitions
    #define NOWINOFFSETS         // Exclude SetWindowLong/GetWindowLong offsets
    #define NOSYSCOMMANDS        // Exclude system command definitions
    #define NOWH                 // Exclude Windows hook APIs
    #define NOCOMM               // Exclude serial communication APIs
    #define NOOPENFILE           // Exclude OpenFile APIs
    #define NOWINERROR           // Exclude error codes
    #define NOWINKEYBOARD        // Exclude keyboard-related definitions
    #define NODRAWTEXT           // Exclude DrawText API
    #define NOMB                 // Exclude MessageBox API
    #define NOMEMMGR             // Exclude GMEM_* and LMEM_* memory management macros
    #define NOMETAFILE           // Exclude metafile functions
    #define NOMSG                // Exclude MSG structure and related functions
    #define NOCLIPBOARD          // Exclude clipboard APIs
    #define NOICONS              // Exclude icon-related APIs
    #define NOKEYSTATES          // Exclude key state APIs
    #define NOSHOWWINDOW         // Exclude ShowWindow API
    #define NODEFERWINDOWPOS     // Exclude DeferWindowPos API
    #define NOVIRTUALKEYCODES    // Exclude virtual key codes
    #define NOWINCONTEXTHELP     // Exclude context help API
    #define NOIMAGE              // Exclude image APIs
    #define NOTAPE               // Exclude tape APIs
    #define NOPROFILER           // Exclude profiler APIs
    #define NOKANJI              // Exclude Kanji support
    #define NOATOM               // Exclude atom manager APIs
    #define NOCRYPT              // Exclude cryptographic APIs
    #define NOKERNEL             // Exclude kernel-related functions
    #define NORESOURCE           // Exclude resource APIs
    #define NOSECURITY           // Exclude security APIs
    #define NOTRACKMOUSEEVENT    // Exclude TrackMouseEvent API
    #include "windows.h"
    #include "platform_win32.cpp"
#elif OS_LINUX // || OS_UNIX || OS_FREEBSD || OS_MAC
    #include <sys/mman.h>
    #include <unistd.h>
#else
    #error "Unsupported OS!"
#endif
    


function vptr allocate_memory(size64 size);
function void free_memory(void* ptr, size_t size);
    
#endif // PLATFORM_H
