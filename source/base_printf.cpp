/* ========================================================================
   $File: base_printf.cpp
   $Date: 2025-03-14
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   $Description: 
    ========================================================================*/


#include "base.h"
#include "base_printf.h"


#include <intrin.h>

static inline
void outChar(FormatDest* dest, const u8 value) {
    if (dest->size) {
        --dest->size;
        *dest->at++ = value;
    }
};


template <typename T>
struct is_format_specifier {
    static constexpr bool value = false;
};

template <>
struct is_format_specifier<FormatSpecifier> {
    static constexpr bool value = true;
};

// Recursive check for all arguments
template <typename... Args>
struct all_are_format_specifiers;

template <>
struct all_are_format_specifiers<> {
    static constexpr bool value = true;
};

template <typename First, typename... Rest>
struct all_are_format_specifiers<First, Rest...> {
    static constexpr bool value = 
        is_format_specifier<First>::value && 
        all_are_format_specifiers<Rest...>::value;
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
FormatSpecifier format(T value) {
    Format f = {};
    FormatSpecifier fs;
#pragma warning(push)
#pragma warning(disable: 4312)
    if constexpr (is_of_type(float)) {
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
void format_cstring(FormatDest* dest, FormatSpecifier spec) {
    u8* str =  (u8*) spec.data;
    while(*str != 0) outChar(dest, *str++);

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

// Move this ?? 
// Returns floor(log_10(2^e)); requires 0 <= e <= 1650.
static inline
u32 log10Pow2(const s32 e) {
  // The first value this approximation fails for is 2^1651 which is just greater than 10^297.
  Assert(e >= 0);
  Assert(e <= 1650);
  return (((u32) e) * 78913) >> 18;
}


// Returns e == 0 ? 1 : ceil(log_2(5^e)); requires 0 <= e <= 3528.
static inline
s32 pow5bits(const s32 e) {
  // This approximation works up to the point that the multiplication overflows at e = 3529.
  // If the multiplication were done in 64 bits, it would fail at 5^4004 which is just greater
  // than 2^9297.
  Assert(e >= 0);
  Assert(e <= 3528);
  return (s32) (((((u32) e) * 1217359) >> 19) + 1);
}

static const u64 DOUBLE_POW5_INV_SPLIT2[15][2] = {
    {                    1u, 2305843009213693952u },
    {  5955668970331000884u, 1784059615882449851u },
    {  8982663654677661702u, 1380349269358112757u },
    {  7286864317269821294u, 2135987035920910082u },
    {  7005857020398200553u, 1652639921975621497u },
    { 17965325103354776697u, 1278668206209430417u },
    {  8928596168509315048u, 1978643211784836272u },
    { 10075671573058298858u, 1530901034580419511u },
    {   597001226353042382u, 1184477304306571148u },
    {  1527430471115325346u, 1832889850782397517u },
    { 12533209867169019542u, 1418129833677084982u },
    {  5577825024675947042u, 2194449627517475473u },
    { 11006974540203867551u, 1697873161311732311u },
    { 10313493231639821582u, 1313665730009899186u },
    { 12701016819766672773u, 2032799256770390445u }
};

static const u32 POW5_INV_OFFSETS[19] = {
    0x54544554, 0x04055545, 0x10041000, 0x00400414, 0x40010000, 0x41155555,
    0x00000454, 0x00010044, 0x40000000, 0x44000041, 0x50454450, 0x55550054,
    0x51655554, 0x40004000, 0x01000001, 0x00010500, 0x51515411, 0x05555554,
    0x00000000
};

#define POW5_TABLE_SIZE 26
static const u64 DOUBLE_POW5_TABLE[POW5_TABLE_SIZE] = {
1ull, 5ull, 25ull, 125ull, 625ull, 3125ull, 15625ull, 78125ull, 390625ull,
1953125ull, 9765625ull, 48828125ull, 244140625ull, 1220703125ull, 6103515625ull,
30517578125ull, 152587890625ull, 762939453125ull, 3814697265625ull,
19073486328125ull, 95367431640625ull, 476837158203125ull,
2384185791015625ull, 11920928955078125ull, 59604644775390625ull,
298023223876953125ull //, 1490116119384765625ull
};




static inline
void double_computeInvPow5(const u32 i, u64* const result) {
    const u32 base = (i + POW5_TABLE_SIZE - 1) / POW5_TABLE_SIZE;
    const u32 base2 = base * POW5_TABLE_SIZE;
    const u32 offset = base2 - i;
    const u64* const mul = DOUBLE_POW5_INV_SPLIT2[base]; // 1/5^base2
    if (offset == 0) {
        result[0] = mul[0];
        result[1] = mul[1];
        return;
    }
    const u64 m = DOUBLE_POW5_TABLE[offset];
    u64 high1;
    const u64 low1 = _umul128(m, mul[1], &high1);
    u64 high0;
    const u64 low0 = _umul128(m, mul[0] - 1, &high0);
    const u64 sum = high0 + low1;
    if (sum < high0) {
        ++high1; // overflow into high1
    }
    // high1 | sum | low0
    const u32 delta = (u32)pow5bits((s32)base2) - (u32)pow5bits((s32)i);
    result[0] = __shiftright128(low0, sum, (u8)delta) + 1 + ((POW5_INV_OFFSETS[i / 16] >> ((i % 16) << 1)) & 3);
    result[1] = __shiftright128(sum, high1, (u8)delta);
}


// It seems to be slightly faster to avoid uint128_t here, although the
// generated code for uint128_t looks slightly nicer.
static inline u32 mulShift32(const u32 m, const u64 factor, const s32 shift) {
  Assert(shift > 32);

  // The casts here help MSVC to avoid calls to the __allmul library
  // function.
  const u32 factorLo = (u32)(factor);
  const u32 factorHi = (u32)(factor >> 32);
  const u64 bits0 = (u64)m * factorLo;
  const u64 bits1 = (u64)m * factorHi;

  const u64 sum = (bits0 >> 32) + bits1;
  const u64 shiftedSum = sum >> (shift - 32);
  // Assert(shiftedSum <= UINT32_MAX);
  return (u32) shiftedSum;
}

static inline u32 mulPow5InvDivPow2(const u32 m, const u32 q, const s32 j) {
  // The inverse multipliers are defined as [2^x / 5^y] + 1; the upper 64 bits from the double lookup
  // table are the correct bits for [2^x / 5^y], so we have to add 1 here. Note that we rely on the
  // fact that the added 1 that's already stored in the table never overflows into the upper 64 bits.
  u64 pow5[2];
  double_computeInvPow5(q, pow5);
  return mulShift32(m, pow5[1] + 1, j);
}


static inline u32 pow5factor_32(u32 value) {
  u32 count = 0;
  for (;;) {
    Assert(value != 0);
    const u32 q = value / 5;
    const u32 r = value % 5;
    if (r != 0) {
      break;
    }
    value = q;
    ++count;
  }
  return count;
}

// Returns true if value is divisible by 5^p.
static inline b8 multipleOfPowerOf5_32(const u32 value, const u32 p) {
  return pow5factor_32(value) >= p;
}

// Returns floor(log_10(5^e)); requires 0 <= e <= 2620.
static inline u32 log10Pow5(const s32 e) {
  // The first value this approximation fails for is 5^2621 which is just greater than 10^1832.
  Assert(e >= 0);
  Assert(e <= 2620);
  return (((u32) e) * 732923) >> 20;
}

static inline u32 mulPow5divPow2(const u32 m, const u32 i, const s32 j) {
#if defined(RYU_FLOAT_FULL_TABLE)
  return mulShift32(m, FLOAT_POW5_SPLIT[i], j);
#elif defined(RYU_OPTIMIZE_SIZE)
  uint64_t pow5[2];
  double_computePow5(i, pow5);
  return mulShift32(m, pow5[1], j);
#else
  return mulShift32(m, DOUBLE_POW5_SPLIT[i][1], j);
#endif
}

// Returns true if value is divisible by 2^p.
static inline b8 multipleOfPowerOf2_32(const u32 value, const u32 p) {
  // __builtin_ctz doesn't appear to be faster here.
  return (value & ((1u << p) - 1)) == 0;
}

// Returns the number of decimal digits in v, which must not contain more than 9 digits.
static inline u32 decimalLength9(const u32 v) {
  // Function precondition: v is not a 10-digit number.
  // (f2s: 9 digits are sufficient for round-tripping.)
  // (d2fixed: We print 9-digit blocks.)
  Assert(v < 1000000000);
  if (v >= 100000000) { return 9; }
  if (v >= 10000000) { return 8; }
  if (v >= 1000000) { return 7; }
  if (v >= 100000) { return 6; }
  if (v >= 10000) { return 5; }
  if (v >= 1000) { return 4; }
  if (v >= 100) { return 3; }
  if (v >= 10) { return 2; }
  return 1;
}

proc inline
#if COMPILER_CL == 1
__declspec(spectre(nomitigation))
#endif
void format_float(FormatDest* dest, FormatSpecifier spec) {
    constexpr u64 FLT_SIZE_MASK = 0b11 << 2;
    const Format formatArg = spec.format;
    if (formatArg.type != FormatType_Real) Assert(false);
 
    (void)dest;

    if ((FLT_SIZE_MASK & formatArg.flags) != FormatFlags_Flt32) {
        Assert(false);
    }

    {
        // Ryu Algorithm 32bit floats 
        FloatUnion floatBits = (*(FloatUnion*)&spec.data);
        IEEE754F32 fValue = (IEEE754F32)floatBits.f;

        // Handle NaN and Inf cases
        /*
          f = (-1)^s * 1.m * 2^(e - bias), iff (0..0) < e < (1..1)    -> Normals 
          f = (-1)^s * 0.m * 2^(e - bias), iff e = (0..0)             -> SubNormals 
          f = (-1)^s * Infinity,           iff e = (1..1), m = 0      -> Infinities
          f = (-1)^s * 1.m * 2^(e - bias), iff (0..0) < e < (1..1)    -> NaN 
          f = (-1)^s0.0                       , iff e = (0..0), m = 0 -> 0.0
         */
        
        constexpr u8 F32_EXPONENT_MAX_VALUE = 0xFF;
        if (fValue.bits.exponent == F32_EXPONENT_MAX_VALUE) { // Inf or NaN
            if (fValue.bits.mantissa == 0) {
                outChar(dest, (u8)(fValue.bits.sign ? '-' : '+'));
                outChar(dest, 'I');
                outChar(dest, 'n');
                outChar(dest, 'f');
                return; 
            }
            outChar(dest, 'N');
            outChar(dest, 'a');
            outChar(dest, 'N');
            return;
        }

        // @INCOMPLETE(HusamD): Is this always true??
        if (fValue.bits.sign) outChar(dest, '-');
        
        // Handle 0.0 and -0.0
        if (fValue.bits.exponent == 0 && fValue.bits.mantissa == 0) {
            outChar(dest, '0');
            outChar(dest, '.');
            outChar(dest, '0');
            return;
        }


        // Normals and Subnormals IEEE 754 floating points,
        constexpr u32 MANTISSA_LEN = 23;
        constexpr u32 BIAS         = 127;
        constexpr u32 MF_ADD       = 1 << (MANTISSA_LEN);
        constexpr s32 FLOAT_POW5_INV_BITCOUNT = 59;
        constexpr s32 FLOAT_POW5_BITCOUNT = 61;
        u32 mf;
        u8 lastRemovedDigit = 0;
        const u32 is_normal = (fValue.bits.exponent != 0);  // 1 if normal, 0 if subnormal

        s32 e2 = (s32)(fValue.bits.exponent + (fValue.bits.exponent == 0)) 
            - (s32)BIAS 
            - (s32)MANTISSA_LEN
            - 2;
        s32 e10 = e2;
        mf = fValue.bits.mantissa + (MF_ADD & -(s32)is_normal);
        
        u32 mmShift = fValue.bits.mantissa == 0 && fValue.bits.exponent > 1;
        u32 u = (4 * mf) - (1 + mmShift);
        u32 v = (4 * mf);
        u32 w = (4 * mf) + 2;

        const b8 even = (mf & 1) == 0;
        const b8 acceptBounds = even;
        b8 vmIsTrailingZeros = false;
        b8 vrIsTrailingZeros = false;
        u32 vr = 0, vp = 0, vm = 0;
        // e2 ≥ 0 => e10 =  0, (a,b,c) = (u,v,w) * 2e2 (keep as is)
        if (e2 >= 0) {
            const u32 q = log10Pow2(e2);
            e10 = (s32) q;
            const s32 k = FLOAT_POW5_INV_BITCOUNT + pow5bits((s32) q) - 1;
            const s32 i = -e2 + (s32) q + k;
            s32 j = (s32) q - k;
            vp = mulPow5InvDivPow2(w, q, i);
            vr = mulPow5InvDivPow2(v, q, i);
            vm = mulPow5InvDivPow2(u, q, i);

            if (q != 0 && (vp - 1) / 10 <= vm / 10) {
                // We need to know one removed digit even if we are not going to loop below. We could use
                // q = X - 1 above, except that would require 33 bits for the result, and we've found that
                // 32-bit arithmetic is faster even on 64-bit machines.
                const u32 l = FLOAT_POW5_INV_BITCOUNT + pow5bits((s32) (q - 1)) - (u32)1;
                lastRemovedDigit = (u8) (mulPow5InvDivPow2(v, q - 1, -e2 + (s32) (q - 1 + l)) % 10);
            }

            if (q <= 9) {
                // The largest power of 5 that fits in 24 bits is 5^10, but q <= 9 seems to be safe as well.
                // Only one of mp, mv, and mm can be a multiple of 5, if any.
                if (v % 5 == 0) {
                    vrIsTrailingZeros = multipleOfPowerOf5_32(v, q);
                } else if (acceptBounds) {
                    vmIsTrailingZeros = multipleOfPowerOf5_32(u, q);
                } else {
                    vp -= multipleOfPowerOf5_32(w, q);
                }
            }
            
        } else {
            // e2 < 0 => e10 = e2, (a,b,c) = (u,v,w) * 5^e2
            const u32 q = log10Pow5(-e2);
            e10 = (s32) q + e2;
            const s32 i = -e2 - (s32) q;
            const s32 k = pow5bits(i) - FLOAT_POW5_BITCOUNT;
            s32 j = (s32) q - k;
            vr = mulPow5divPow2(v, (u32) i, j);
            vp = mulPow5divPow2(w, (u32) i, j);
            vm = mulPow5divPow2(u, (u32) i, j);

            if (q != 0 && (vp - 1) / 10 <= vm / 10) {
                j = (s32) q - 1 - (pow5bits(i + 1) - FLOAT_POW5_BITCOUNT);
                lastRemovedDigit = (u8) (mulPow5divPow2(v, (u32) (i + 1), j) % 10);
            }
            if (q <= 1) {
                // {vr,vp,vm} is trailing zeros if {mv,mp,mm} has at least q trailing 0 bits.
                // mv = 4 * m2, so it always has at least two trailing 0 bits.
                vrIsTrailingZeros = true;
                if (acceptBounds) {
                    // mm = mv - 1 - mmShift, so it has 1 trailing 0 bit iff mmShift == 1.
                    vmIsTrailingZeros = mmShift == 1;
                } else {
                    // mp = mv + 2, so it always has at least one trailing 0 bit.
                    --vp;
                }
            } else if (q < 31) { // TODO(ulfjack): Use a tighter bound here.
                vrIsTrailingZeros = multipleOfPowerOf2_32(v, q - 1);
            }
        }


        // Step 4: Find the shortest decimal representation in the interval of valid representations.
        s32 removed = 0;
        u32 output;
        if (vmIsTrailingZeros || vrIsTrailingZeros) {
            // General case, which happens rarely (~4.0%).
            while (vp / 10 > vm / 10) {

                vmIsTrailingZeros &= vm % 10 == 0;
                vrIsTrailingZeros &= lastRemovedDigit == 0;
                lastRemovedDigit = (u8) (vr % 10);
                vr /= 10;
                vp /= 10;
                vm /= 10;
                ++removed;
            }
            if (vmIsTrailingZeros) {
                while (vm % 10 == 0) {
                    vrIsTrailingZeros &= lastRemovedDigit == 0;
                    lastRemovedDigit = (u8) (vr % 10);
                    vr /= 10;
                    vp /= 10;
                    vm /= 10;
                    ++removed;
                }
            }

            if (vrIsTrailingZeros && lastRemovedDigit == 5 && vr % 2 == 0) {
                // Round even if the exact number is .....50..0.
                lastRemovedDigit = 4;
            }
            // We need to take vr + 1 if vr is outside bounds or we need to round up.
            output = vr + ((vr == vm && (!acceptBounds || !vmIsTrailingZeros)) || lastRemovedDigit >= 5);
        } else {
            // Specialized for the common case (~96.0%). Percentages below are relative to this.
            // Loop iterations below (approximately):
            // 0: 13.6%, 1: 70.7%, 2: 14.1%, 3: 1.39%, 4: 0.14%, 5+: 0.01%
            while (vp / 10 > vm / 10) {
                lastRemovedDigit = (u8) (vr % 10);
                vr /= 10;
                vp /= 10;
                vm /= 10;
                ++removed;
            }

            // We need to take vr + 1 if vr is outside bounds or we need to round up.
            output = vr + (vr == vm || lastRemovedDigit >= 5);
        }
        const s32 exp = e10 + removed; 

        // TODO: Fix me, this is absurd
        //       The problem here is that the original algorithm is writing the digits out of order
        u32 decimalPointLocation = decimalLength9(output);
        u32 olength = decimalPointLocation;
#if 1
        u32 i = 0;
        while (output >= 10000) {
            const u32 c = output % 10000;
            output /= 10000;
            const u32 c0 = (c % 100) << 1;
            const u32 c1 = (c / 100) << 1;

#if MY_PRINT
            const u8* c1Digits = DIGIT_TABLE + c1;
            outChar(dest, *(c1Digits + 0));
            outChar(dest, *(c1Digits + 1));
            
            const u8* c0Digits = DIGIT_TABLE + c0;
            outChar(dest, *(c0Digits + 0));
            outChar(dest, *(c0Digits + 1));
#else
            memcpy(dest->at + dest->size + olength - i - 1, DIGIT_TABLE + c0, 2);
            memcpy(dest->at + dest->size + olength - i - 3, DIGIT_TABLE + c1, 2);
            dest->size += 4;
#endif
            i += 4;
        }
        if (output >= 100) {
            const u32 c = (output % 100) << 1;
            output /= 100;

#if MY_PRINT            
            const u8* cDigits = DIGIT_TABLE + c;
            outChar(dest, *(cDigits + 0));
            outChar(dest, *(cDigits + 1));
#else            
            memcpy(dest->at + dest->size + olength - i - 1, DIGIT_TABLE + c, 2);
            dest->size += 2;
#endif
            i += 2;
        }

        if (output >= 10) {
            const u32 c = output << 1;

            const u8* cDigits = DIGIT_TABLE + c;
            outChar(dest, DIGIT_TABLE[c]);
            outChar(dest, DIGIT_TABLE[c + 1]);
            
            // We can't use memcpy here: the decimal dot goes between these two digits.
            // result[dest->at + olength - i] = DIGIT_TABLE[c + 1];
            // result[dest->at] = DIGIT_TABLE[c];
            // dest->size += 2;
        } else {
            outChar(dest, '0');
        }
#else
        // u32 power1 = power(10, decimalPointLocation - ;)1
        // u32 left = output / power1;
        // format_int(dest, format(output));
#endif
        // outChar(dest, '.');
        // format_int(dest, format(output));
    }
    return;
}

#if 0
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

        if (!frac_part) {
            return;
        }
        
        outChar(dest, '.');

        s64 negartive_real_exponent = -real_exponent;
        
        for (u32 i = 0; i < 10; ++i) { // Print up to 6 decimal places
            frac_part *= 10;
            auto digit = (frac_part >> negartive_real_exponent);
            if (digit < sizeof(Digits) && digit > 0)
                outChar(dest, (u8)Digits[digit]);
            frac_part &= ((1ULL << negartive_real_exponent) - 1);
            if (frac_part == 0) break;
        }
#endif        

typedef s8* arg_list;

#if COMPILER_CL == 1

/*
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
#define args_arg(ap, t) _t_args_arg<t>(ap)
*/
#define _args_arg(ap, t)                                                \
    ((sizeof(t) > sizeof(__int64) || (sizeof(t) & (sizeof(t) - 1)) != 0) \
     ? **(t**)((ap += sizeof(__int64)) - sizeof(__int64))               \
     :  *(t* )((ap += sizeof(__int64)) - sizeof(__int64)))
#define args_arg _args_arg
#define args_start(ap, v) ((void)(__va_start(&ap, v)))
#define args_end(ap)    ((void)(ap = (arg_list)0))
#else
#error Please define variable arguments macros
#endif


void process_args(FormatDest* dest, const u8* format) {
    // Output remaining format string
    while (*format) {
        outChar(dest, *format++);
    }
}

#if 0
proc
u64 printf(u8* dest, u64 destSize, string8 formatString, ...) {
   
    if (!destSize) return 0;
    
    FormatDest formatDest = {destSize, dest};

    arg_list args;
    args_start(args, formatString);
    
    u8* at = formatString.str;
    while(at[0]) {
        if (*at != '%') {
            outChar(&formatDest, *at++);
            continue;
        }

        at++;
        FormatSpecifier specifier = args_arg(args, FormatSpecifier);
        switch (specifier.format.type) {
          case FormatType_Integer: format_int(&formatDest, specifier); break;
          case FormatType_Real: format_float(&formatDest, specifier); break;
          case FormatType_CString:{
              u8* str =  (u8*) specifier.data;
              while(*str != 0) outChar(&formatDest, *str++);
          }break;
          case FormatType_String8: {
              string8* str =  (string8*) specifier.data;
              u64 bytesToCopy = str->size;
              u64 bytesWritten = 0;
              while(bytesToCopy--) {
                  outChar(&formatDest, str->str[bytesWritten++]);
              }
          }break;
          case FormatType_None:
          default: break;
        };
    };

    args_end(args);
    return (u64)(destSize - formatDest.size);

}
#endif 

template <typename... Args>
static
void process_format(FormatDest* dest, const u8* format, Args... args) {
    FormatSpecifier specifiers[] = {args...};  // Pack into array

    u64 specifierIndex = 0;
    while (*format) {
        if (*format != '%') {
            outChar(dest, *format++);
            continue;
        }

        format++;
        if (specifierIndex >= sizeof...(args)) {
            Assert(false);
            // panic("Too few FormatSpecifiers!");
        }

        FormatSpecifier specifier = specifiers[specifierIndex++];
        switch (specifier.format.type) {
          case FormatType_Integer : format_int(dest, specifier); break;
          case FormatType_Real    : format_float(dest, specifier); break;
          case FormatType_CString : format_cstring(dest, specifier); break;
          case FormatType_String8 : // fallthrough
          case FormatType_None    : // fallthrough
          default                 : break; // Do nothing 
        }
    }

#if 0
    case FormatType_String8: {
        string8* str =  (string8*) specifier.data;
        u64 bytesToCopy = str->size;
        u64 bytesWritten = 0;
        while(bytesToCopy--) {
            outChar(dest, str->str[bytesWritten++]);
        }
    }break;
#endif
    // TODO: Can we do this at compile time ?
    /* 
    if (specifierIndex != sizeof...(args)) {
        Assert(false);
        // panic("Too many FormatSpecifiers!");
    }
    */
}


template <typename... Args>
u64 printf(u8* dest, u64 destSize, string8 formatString, Args... args) {
    if constexpr (!all_are_format_specifiers<Args...>::value) {
        []<bool flag = false>()
        {static_assert(flag, "All arguments must be of type FormatSpecifier!");}();
    }
    
    if (!destSize) return 0;
    
    FormatDest formatDest = {destSize, dest};
    process_format(&formatDest, formatString.str, args...);
    return destSize - formatDest.size;
}

/*
// https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGIM6SuADJ4DJgAcj4ARpjEEgEADqgKhE4MHt6%2B/qRJKY4CIWGRLDFxXAF2mA5pQgRMxAQZPn5ctpj2%2BQy19QSFEdGxegp1DU1ZrcM9fcWlEgCUtqhexMjsHF4MKcBh6ADUoQS7DBAbWzv7grtMcyYaAIImAOxW97tvp3jbmHsHu1G7JgAzAARXYQLgAwFuIFuXZmQFzAGWMFMJEANl2GlUjwAYni8TdAS87m9dsRMARlgw/kDiU9gbcHvdGR8vj9LgwzCdNp9zr9rgDmc9GaTdqy%2BZd/kDQRBUeYMVjcficYinrCsQAODTa7W0kWk8mU4jUqJ6oXAjgLWicACsvD8HC0pFQnGhlmsuwUSxWmCRgJ4pAImktCwA1v4AJwAOjRkjMkhtcYjNrMGjMaMBaP0nEk9uDzs4vAUIA0geDCzgsCQaBYCTosXIlBrdfocQAbsgEgkAPptrgR7sGAiYYbd1Sxvh0YfEYsQKL5qKheoAT04AcXzGIy4A8lFtFUg9xeDW2IJtwxaKvHbwsFEvMA3GJaMWj6QsCxDMBxNe33hydU2xHfNMFUKovGHNdeAOdp81oPAomIFcPCwSDA2IPAWFQwDiCiZJMGBTAPyMOCjHLPgDGABQADU8EwAB3bcEkYVD%2BEEEQxHYKQZEERQVHUH9dFaAxSNMd1LH0eDi0gBZUASTpiwAem3VRnWw9CsCkiAFkqapnAgVwxhaQIGHQaYBjKHJklSARDL0XJrIYMySkGCZ2gPGoRkaTxmiGNzdK6TynNmCZPNskKplCfpnLKbTvVWeZsw4O1SAdJ0XQ4XZx0kXYWAUDtdj7aMhxHQ4IFwQgSD9Lg5l4Q8tDmMN/DMKMIwjDVJA1MwIzMMwuH7Lg0Sza0OFzUhMK4bUUvzdKixLMtrwrGBEBQVBa3rMgKAgZt1pAYApACGhaGnWd5x/DcV1Q86t13fcHFQk9GAIc9L3zW970fWhn1Q99P2/J18H/RxAJfJ0QLAiDX2g4anTghCkIwNYnQIdDMNfbDcKUAiiK/UJQAW8imEomj6MY5jX1Y4RRHELiKd4tR810AJhOMaxrAkqJNJkuS0hfABabdAV4VA1LwDT4G0vzOhcEywuM0zIpmFzLLyNJZfszogqVnTOm6UZvPGNoOg8iKinMoZQv1ozJgaTWYsWZZ4uqxLktSoXOEyjU0V52NdmAZBkF2KQozMMFyqIYgqpq%2Bb6sa%2BEY0eZN/UBCMuEBZOhJzXhxsm12Cw4WbSzqy1SErZbttbRsttWltBmQZnUwmycjtiE6FyXLdLvbnc9wPe7VtPJ6Lyvf7MDvB8nxfAMfuIxGbz/dzgeA0DkHAtYAyh2D4MQrdkNntCMKw2IMfwwjftxsiqAo6jaIYpiHQDCn2Op6RaaUemBJAQF9E/FBWfE2HObOm5gIPmAshYizFtJQ27k9IGUtnoYICszatHVqreBKCrIayQdFXyRsBC6y8pkK2ktjY22wcFWwFsiHmxNlFYKsUHacStLaPMP50oey9j7Oun44QaCjFwKMGhQ74HDpHWq5YFgIEwEwLAcQtKkHDHHNECcbRJxTmnFOiVRq5xmrYOaRcGqJTMFnCQOdpqFmjiGUg2EUjOEkEAA%3D
unsigned int n(unsigned int a)
{
   unsigned int b = (1 << 23) + a;
   return b;
}


unsigned int n2(unsigned int a) 
{
    unsigned int b = (a & 0x7FFFFF) | 0x800000;
    return b;
}
*/
