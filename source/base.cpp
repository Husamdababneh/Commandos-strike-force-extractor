/* ========================================================================
   $File: base.cpp
   $Date: 2025-03-03
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   $Description: 
    ========================================================================*/


#include "base.h" 

proc
f32 abs_f32(f32 x) {
    union{f32 f; u32 u;} r;
    r.f  = x;
    r.u &= SignFlipper32;
    return r.f;
}


proc
f64 abs_f64(f64 x) {
    union{f64 f; u64 u;} r;
    r.f  = x;
    r.u &= SignFlipper64;
    return r.f;
}

//~ Strings
proc
string8 operator""_s8(const char* str, size_t length) {
    string8 result;
    result.str  = (u8*)str;
    result.size = length;
    return result;
}

// TODO: Move to Base Math
proc
u64 power(u32 base, u32 exp) {
    u64 result = 1; // Use a larger type to avoid overflow

    while (exp) {
        if (exp & 1) result *= base; // Multiply when the lowest bit is set
        base *= base;  // Square the base
        exp >>= 1;     // Divide exponent by 2 (shift right)
    }

    return result;
}

#define CFUNC_START extern "C" {
#define CFUNC_END   }


// #pragma intrinsic(memset, memcpy)
