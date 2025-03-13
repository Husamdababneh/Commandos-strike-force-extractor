/* ========================================================================
   $File: base_printf.h
   $Date: 2025-03-14
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   $Description: 
    ========================================================================*/

#ifndef BASE_PRINTF_H
#define BASE_PRINTF_H


enum FormatType : u8 {
    FormatType_CString,
    FormatType_String8,
    FormatType_Integer,
    FormatType_Real
};

enum FormatFlags: u64 {
    // **Common Bit Reuse for Different Types**
    FormatFlags_Uppercase       = 1 << 0,  // Strings: Convert to uppercase
    FormatFlags_Lowercase       = 1 << 1,  // Strings: Convert to lowercase
    FormatFlags_TrimWhitespace  = 1 << 2,  // Strings: Trim leading/trailing spaces
    
    FormatFlags_Scientific      = 1 << 0,  // Floats: Use scientific notation
    FormatFlags_FixedPoint      = 1 << 1,  // Floats: Use fixed decimal format
    FormatFlags_Compact         = 1 << 2,  // Floats: Remove trailing zeros

    FormatFlags_Hex             = 1 << 0,  // Integers: Print in hexadecimal
    FormatFlags_Octal           = 1 << 1,  // Integers: Print in octal
    FormatFlags_Binary          = 1 << 2,  // Integers: Print in binary

    // **Integer Size Flags (Shared across Signed and Unsigned Integers)**
    FormatFlags_Int8            = 1 << 3,
    FormatFlags_Int16           = 1 << 4,
    FormatFlags_Int32           = 1 << 5,
    FormatFlags_Int64           = 1 << 6,
    FormatFlags_Unsigned        = 1 << 7,  // Mark as unsigned integer

    // **Precision Control for Floating-Point (Bits 8-11)**
    FormatFlags_PrecisionMask   = 0xF << 8,  

    // **Justification (2-bit field in Bits 12-13)**
    FormatJustify_Right         = 0b00 << 12,  // Default: Right-justified
    FormatJustify_Left          = 0b01 << 12,  // Left-justified
    FormatJustify_Center        = 0b10 << 12,  // Center-justified
    // **(Future Feature) Justify Fill (Commented)**
    // The idea is to evenly distribute padding characters **between** content characters.
    // Uncomment this if we later decide to implement this feature.
    // FormatJustify_Fill         = 0b11 << 12,  // Experimental: Distribute padding evenly (e.g., H.e.l.l.o.)
};

struct Format {
    Format* next;

    FormatType type;
    u8 padding[7]; // This is here for the Struct alignement

    vptr data;
    FormatFlags flags;
};

#define StringFormat(x) {nullptr, FormatType_String, (vptr) x##_s8, 0};

struct FormatDest {
    u64 size;
    u8* at;
};



function void outChar(FormatDest* dest, u8 value);
function u64 printf(u8* dest, u64 destSize, string8 formatString, Format* formats);

#endif BASE_PRINTF_H
