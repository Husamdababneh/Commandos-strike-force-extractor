/* ========================================================================
   $File: base_printf.cpp
   $Date: 2025-03-14
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   $Description: 
    ========================================================================*/


#include "base.h"
#include "base_printf.h"



static inline
void outChar(FormatDest* dest, u8 value) {
    if (dest->size) {
        --dest->size;
        *dest->at++ = value;
    }
};

static inline
u64 printf(u8* dest, u64 destSize, string8 formatString, Format* formats) {
    if (!destSize) return 0;

    FormatDest formatDest = {destSize, dest};

    // Fast route if no specifiers
    // This is only strcpy
    if (!formats) {
        // TODO: NOT IMPLEMENTED
        return 0;
    }
   
    Format* current = formats;
    u8* at = formatString.str;
    while(at[0]) {
        if (*at == '%' && current) {
            // TODO:
            at++;
            switch (current->type) {
              case FormatType_CString: break;
              case FormatType_Integer: break;
              case FormatType_Real:  break;
              case FormatType_String8: {
                  string8* str =  (string8*) current->data;
                  u64 bytesToCopy = str->size;
                  u64 bytesWritten = 0;
                  while(bytesWritten < bytesToCopy && formatDest.size > 0)
                      outChar(&formatDest, str->str[bytesWritten++]);
              }break;
            };
            current = current->next;
        } else {
            outChar(&formatDest, *at++);
        }
    };
    return (u64)(destSize - formatDest.size);
}
