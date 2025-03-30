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
    FormatType_None = 0,
    FormatType_CString,
    FormatType_String8,
    FormatType_Integer,
    FormatType_Real
};


// TODO: Rethink this because I'm mixing flags that describes the value with flags of how to print value
enum FormatFlags: u64 {
    // **Common Bit Reuse for Different Types**
    FormatFlags_Str_None            = 0b00 << 0,  // Strings: keep string as is
    FormatFlags_Str_Uppercase       = 0b01 << 0,  // Strings: Convert to uppercase
    FormatFlags_Str_Lowercase       = 0b10 << 0,  // Strings: Convert to lowercase
    FormatFlags_Str_TrimWhitespace  = 0b11 << 0,  // Strings: Trim leading/trailing spaces

    FormatFlags_FLT_RESERVED        = 0b00 << 0,  // Floats: RESERVED
    FormatFlags_FLT_Scientific      = 0b01 << 0,  // Floats: Use scientific notation
    FormatFlags_FLT_FixedPoint      = 0b10 << 0,  // Floats: Use fixed decimal format
    FormatFlags_FLT_Compact         = 0b11 << 0,  // Floats: Remove trailing zeros


    FormatFlags_INT_Decimal         = 0b00 << 0,  // Integers: Print in decimal
    FormatFlags_INT_Hex             = 0b01 << 0,  // Integers: Print in hexadecimal
    FormatFlags_INT_Octal           = 0b10 << 0,  // Integers: Print in octal
    FormatFlags_INT_Binary          = 0b11 << 0,  // Integers: Print in binary
    
    // NOTE: Integer/Float Size Flags (Shared across Signed and Unsigned Integers) (Bits 2-4)**
    // bits 3-4

    FormatFlags_Flt16           = 0b00 << 2,
    FormatFlags_Flt32           = 0b01 << 2,
    FormatFlags_Flt64           = 0b10 << 2,
    FormatFlags_Flt128          = 0b11 << 2,
    
    FormatFlags_Int8            = 0b00 << 2,
    FormatFlags_Int16           = 0b01 << 2,
    FormatFlags_Int32           = 0b10 << 2,
    FormatFlags_Int64           = 0b11 << 2,
    
    FormatFlags_Unsigned        = 1 << 4,  // Mark as unsigned integer

    // **Precision Control for Floating-Point (Bits 5-8)**
    FormatFlags_PrecisionMask   = 0xF << 5,

    // **Justification (2-bit field in Bits 9-10)**
    FormatFlags_Justify_Right         = 0b00 << 9,  // Default: Right-justified
    FormatFlags_Justify_Left          = 0b01 << 9,  // Left-justified
    FormatFlags_Justify_Center        = 0b10 << 9,  // Center-justified
    // **(Future Feature) Justify Fill (Commented)**
    // The idea is to evenly distribute padding characters **between** content characters.
    // Uncomment this if we later decide to implement this feature.
    // FormatJustify_Fill         = 0b11 << 10,  // Experimental: Distribute padding evenly (e.g., H.e.l.l.o.)
};

struct Format {  
    FormatFlags flags;
    FormatType type;
    Padding8 pad[7];
};

struct FormatSpecifier {
    Format format;
    vptr          data;
};


struct FormatDest {
    u64 size;
    u8* at;
};


typedef union {
    f32 value;
    struct {
        u32 mantissa : 23;
        u32 exponent : 8;
        u32 sign     : 1;
    } bits;
} IEEE754F32;

typedef union {
    f64 value;
    struct {
        u64 mantissa : 52;
        u64 exponent : 11;
        u64 sign     : 1;
    } bits;
} IEEE754F64;


proc void outChar(FormatDest* dest, u8 value);
proc u64 printf(u8* dest, u64 destSize, string8 formatString, FormatSpecifier* formats);

#endif BASE_PRINTF_H
