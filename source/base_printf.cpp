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
void outChar(FormatDest* dest, const u8 value) {
    if (dest->size) {
        --dest->size;
        *dest->at++ = value;
    }
};


template <typename T, typename U>
struct is_same {
    static const bool value = false;
};

template <typename T>
struct is_same<T, T> {
    static const bool value = true;
};

// Helper template to produce a dependent false.
template <typename T>
struct dependent_false {
    static const bool value = false;
};

#define is_of_type(x) is_same<T, x>::value
  
template<typename T>
constexpr FormatSpecifier format(T value) {
    Format f = {};
    FormatSpecifier fs;
#pragma warning(push)
#pragma warning(disable: 4312)
    if constexpr (is_of_type(string8)) {
        f.flags = (FormatFlags)(FormatFlags_Str_None);
        f.type = FormatType_String8;
        fs.data = (vptr)value;
    } else if constexpr (is_of_type(float)) {
        f.flags = (FormatFlags)(FormatFlags_Flt32);
        f.type = FormatType_Real;
        FloatUnion* newVlaue = (FloatUnion*)&value;
        fs.data = (vptr)newVlaue->i;
    } else if constexpr (is_of_type(double)) {
        f.flags = (FormatFlags)(FormatFlags_Flt64);
        f.type = FormatType_Real;
        DoubleUnion* newVlaue = (DoubleUnion*)&value;
        fs.data = (vptr)newVlaue->i;
    } else if constexpr (is_of_type(u8)) {
        f.flags = (FormatFlags)(FormatFlags_INT_Decimal | FormatFlags_Int8 | FormatFlags_Unsigned);
        f.type = FormatType_Integer;
        fs.data = (vptr)value;
    } else if constexpr (is_of_type(u16)) {
        f.flags = (FormatFlags)(FormatFlags_INT_Decimal | FormatFlags_Int16 | FormatFlags_Unsigned);
        f.type = FormatType_Integer;
        fs.data = (vptr)value;
    } else if constexpr (is_of_type(u32)) {
        f.flags = (FormatFlags)(FormatFlags_INT_Decimal | FormatFlags_Int32 | FormatFlags_Unsigned);
        f.type = FormatType_Integer;
        fs.data = (vptr)value;
    } else if constexpr (is_of_type(u64)) {
        f.flags = (FormatFlags)(FormatFlags_INT_Decimal | FormatFlags_Int64 | FormatFlags_Unsigned);
        f.type = FormatType_Integer;
        fs.data = (vptr)value;
    } else if constexpr (is_of_type(s8)) {
        f.flags = (FormatFlags)(FormatFlags_INT_Decimal | FormatFlags_Int8);
        f.type = FormatType_Integer;
        fs.data = (vptr)value;
    } else if constexpr (is_of_type(s16)) {
        f.flags = (FormatFlags)(FormatFlags_INT_Decimal | FormatFlags_Int16);
        f.type = FormatType_Integer;
        fs.data = (vptr)value;
    } else if constexpr (is_of_type(s32)) {
        f.flags = (FormatFlags)(FormatFlags_INT_Decimal | FormatFlags_Int32);
        f.type = FormatType_Integer;
        fs.data = (vptr)value;
    } else if constexpr (is_of_type(s64)) {
        f.flags = (FormatFlags)(FormatFlags_INT_Decimal | FormatFlags_Int64);
        f.type = FormatType_Integer;
        fs.data = (vptr)value;
    } else if constexpr (is_of_type(int)) {
        f.flags = (FormatFlags)(FormatFlags_INT_Decimal | FormatFlags_Int32);
        f.type = FormatType_Integer;
        fs.data = (vptr)value;
    } else if constexpr(is_of_type(const char *)) {
        f.flags = (FormatFlags)(FormatFlags_Str_None);
        f.type = FormatType_CString;
        fs.data = (vptr)value;
    } else {
        []<bool flag = false>()
        {static_assert(flag, "no match");}();
    }
    
#pragma warning(pop)

    fs.format = f;
    return fs;
}


proc inline
void format_int(FormatDest* dest, FormatSpecifier spec) {
    const Format format = spec.format;
    // Number of characters a 128 bit unsined integer will have + 1 for singed
    constexpr const char* Digits = "0123456789";
    constexpr u64 MAX_DIGIT_COUNT = 39 + 1;
    constexpr u64 INT_SIZE_MASK = 0b11 << 2;
    u8 temp[MAX_DIGIT_COUNT] = {};
    u8 counter = 0;

#pragma warning(push)
#pragma warning(disable: 4302 4311)
    if (format.flags & FormatFlags_Unsigned) {
        u64 integer = 0;
        switch(INT_SIZE_MASK & format.flags) {
          case FormatFlags_Int8:  {integer =  u64((u8)  spec.data);} break;
          case FormatFlags_Int16: {integer =  u64((u16) spec.data);} break;
          case FormatFlags_Int32: {integer =  u64((u32) spec.data);} break;
          case FormatFlags_Int64: {integer =  u64((u64) spec.data);} break;
        };
        do {
            u64 r  = integer % 10;
            integer = integer / 10;
            temp[counter++] = (u8)Digits[r];
        } while(integer);

        while(counter--) outChar(dest, (u8)temp[counter]);
    } else {
        s64 integer = 0;
        // Handle MIN values for integers
        switch(INT_SIZE_MASK & format.flags) {
          case FormatFlags_Int8:  {integer =  s64((s8)  spec.data);} break;
          case FormatFlags_Int16: {integer =  s64((s16) spec.data);} break;
          case FormatFlags_Int32: {integer =  s64((s32) spec.data);} break;
          case FormatFlags_Int64: {integer =  s64((s64) spec.data);} break;
        };
        if (integer < 0) {
            outChar(dest, (u8)'-');
            integer = -integer;  // Take absolute value
        }
        
        do {
            s64 r  = integer % 10;
            integer = integer / 10;
            temp[counter++] = (u8)Digits[r];
        } while(integer);

        while(counter--) outChar(dest, (u8)temp[counter]);
    }
#pragma warning(pop)
}


proc inline
void format_float(FormatDest* dest, FormatSpecifier spec) {
    constexpr u64 FLT_SIZE_MASK = 0b11 << 2;
    const Format format = spec.format;
    if (format.type != FormatType_Real) Assert(false);
 
    (void)dest;

    if ((FLT_SIZE_MASK & format.flags) != FormatFlags_Flt32) {
        Assert(false);
    }

    {
        // Ryu Algorithm
        FloatUnion floatBits = (*(FloatUnion*)&spec.data);
        IEEE754F32 fValue = (IEEE754F32)floatBits.f;

        s32 exponent = (s32)(fValue.bits.exponent & 0xFF) - 127;
        u32 mantissa = (fValue.bits.mantissa & 0x7FFFFF) | 0x800000;
        // Handle special cases
        if (exponent == 128) { // Inf or NaN
            if (mantissa == 0) {
                outChar(dest, (u8)(fValue.bits.sign ? '-' : '+'));
                outChar(dest, 'I');
                outChar(dest, 'n');
                outChar(dest, 'f');
                return; 
            } else {
                outChar(dest, 'N');
                outChar(dest, 'a');
                outChar(dest, 'N');
                return;
            }
            return;
        }

        if (fValue.bits.sign) outChar(dest, '-');

        // Convert to integer representation
        u64 real_mantissa = mantissa;
        s64 real_exponent = exponent - 23;

        // Convert integer part
        u64 int_part = real_mantissa >> (-real_exponent);
        u64 frac_part = real_mantissa & ((1ULL << (-real_exponent)) - 1);

        constexpr u64 MAX_DIGIT_COUNT = 20;
        constexpr const char* Digits = "0123456789";
        u8 temp[MAX_DIGIT_COUNT] = {};
        u8 counter = 0;
        do {
            u64 r  = int_part % 10;
            int_part = int_part / 10;
            temp[counter++] = (u8)Digits[r];
        } while(int_part);
        
        // Reverse intgers
        while(counter--) outChar(dest, (u8)temp[counter]);

        if (frac_part) {
            outChar(dest, '.');
            for (int i = 0; i < 6; ++i) { // Print up to 6 decimal places
                frac_part *= 10;
                auto digit = (frac_part >> (-real_exponent));
                outChar(dest, (u8)Digits[digit]);
                frac_part &= ((1ULL << (-real_exponent)) - 1);
                if (frac_part == 0) break;
            }
        }
    }
}


typedef s8* arg_list;

#if COMPILER_CL == 1

template<typename T>
constexpr T _t_args_arg(char*& ap) {
    // Check: if T is larger than __int64 or its size is not a power of two,
    // then we expect that a pointer to T was stored (hence a double dereference).
    if constexpr ( sizeof(T) > sizeof(__int64) ||
                   ((sizeof(T) & (sizeof(T) - 1)) != 0) ) {
        // Increment pointer by sizeof(__int64)
        ap += sizeof(__int64);
        // The stored value is assumed to be a pointer to T.
        return **(reinterpret_cast<T**>(ap - sizeof(__int64)));
    } else {
        ap += sizeof(__int64);
        // For types small enough and whose sizes are a power of two,
        // the value is stored directly.
        return *(reinterpret_cast<T*>(ap - sizeof(__int64)));
    }
}
/*
#define _args_arg(ap, t)                                                \
    ((sizeof(t) > sizeof(__int64) || (sizeof(t) & (sizeof(t) - 1)) != 0) \
     ? **(t**)((ap += sizeof(__int64)) - sizeof(__int64))               \
     :  *(t* )((ap += sizeof(__int64)) - sizeof(__int64)))
#define args_arg _args_arg
*/
#define args_arg(ap, t) _t_args_arg<t>(ap)
#define args_start(ap, v) ((void)(__va_start(&ap, v)))
#define args_end(ap)    ((void)(ap = (arg_list)0))
#else
#error Please define variable arguments macros
#endif


proc  
u64 printf(u8* dest, u64 destSize, string8 formatString, ...) {

   
    arg_list args;
    args_start(args, formatString);
    
    if (!destSize) return 0;
    
    FormatDest formatDest = {destSize, dest};
    
    //Format* current = nullptr;
    u8* at = formatString.str;
    while(at[0]) {
        if (*at != '%') {
            outChar(&formatDest, *at++);
            continue;
        }

        at++;
        FormatSpecifier specifier = args_arg(args, FormatSpecifier);
        switch (specifier.format.type) {
          case FormatType_Integer: {
              format_int(&formatDest, specifier);
          }break;
          case FormatType_Real: {
              format_float(&formatDest, specifier);
          }break;
          case FormatType_CString:{
              u8* str =  (u8*) specifier.data;
              while(*str != 0) outChar(&formatDest, *str++);
          }break;
          case FormatType_String8: {
              string8* str =  (string8*) specifier.data;
              u64 bytesToCopy = str->size;
              // memcpy(&formatDest.at, str->str, bytesToCopy);
              u64 bytesWritten = 0;
              while(bytesToCopy--) {
                  outChar(&formatDest, str->str[bytesWritten++]);
              }
              // formatDest.size -= bytesToCopy;
          }break;
          case FormatType_None:
          default: break;
        };
    };

    args_end(args);
    return (u64)(destSize - formatDest.size);

}
