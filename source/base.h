/* ========================================================================
   $File: base.h
   $Date: 2025-03-02
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   $Description: Base Layer, this contains 
   ========================================================================*/

#ifndef BASE_H
#define BASE_H

#if !defined(__cplusplus)
#error C++ Compiler Must be used
#endif

//~ 
// NOTE(Husam Dababneh): Some useful Macros

// Casey Muratori way of differentiating between the meaning of static in different locations
// Static Local variable
#define local_presist static 
// Global variable
#define global_variable static 
// translation-unit scope function
#define internal static

#define function static

#define offsetof(a, b) (u64)(&((a*)0)->b)
#define member_size(type, member) (u64)(sizeof(((type *)0)->member))
#define countof(x) sizeof(x) / sizeof(x[0])

#define bool_value(x) x ? "true" : "false"

#define stringify_literal( x ) # x
#define contact(x, y) #x ": " stringify_expanded(y)
#define contact2(x, y) #x ": " stringify_with_quotes(y)
#define stringify_expanded( x ) stringify_literal( x )
#define stringify_with_quotes( x ) stringify_expanded( stringify_expanded( x ) )

#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING STRINGIZE(__LINE__)

#define alignPow2(x,b)     (((x) + (b) - 1)&(~((b) - 1)))
#define alignDownPow2(x,b) ((x)&(~((b) - 1)))
#define alignPadPow2(x,b)  ((0-(x)) & ((b) - 1))
#define isPow2(x)          ((x)!=0 && ((x)&((x)-1))==0)
#define isPow2OrZero(x)    ((((x) - 1)&(x)) == 0)

// NOTE: I think this makes the code more readable when used 
#define isEven(x) (((x) & 1) == 0)
#define isOdd(x)  (((x) & 1) != 0)

#if defined(_MSC_VER) && !defined(__clang__)
# define COMPILER_CL 1 // MSVC Default Compiler
# define COMPILER_NAME contact2(Microsoft Visual C++ Compiler (CL) Version, _MSC_FULL_VER)
# if defined(_M_AMD64) || defined(_M_X64)
#  define ARCH_AMD64 1
#  define ARCH_NAME "AMD 64-bit Architecture"
# elif defined(_M_ARM)
#  define ARCH_ARM64   1
#  define ARCH_NAME "ARM Architecture"
# elif defined(_M_I86)
#  define ARCH_INTEL32   1
#  define ARCH_NAME "Intel 16-Bit Architecture"
# elif defined(_M_IX86)
#  define ARCH_INETL64   1
#  define ARCH_NAME "Intel 32-Bit Architecture"
# endif 
#elif defined(_MSC_VER) && defined(__clang__)
# define COMPILER_CLANG 1
# define COMPILER_NAME contact(Clang C++ Compiler (Clang) Version, __clang_version__)
# if defined(__x86_64__)
#  define ARCH_AMD64 1
#  define ARCH_NAME "AMD 64-bit Architecture"
// not TESTED
# elif defined(__arm__)
#  define ARCH_ARM32   1
#  define ARCH_NAME "ARM 32-bit Architecture"
# elif defined(__aarch64__)
#  define ARCH_ARM64   1
#  define ARCH_NAME "ARM 64-bit Architecture"
# endif
#elif defined(__GNUC__)
# error NOT COMPLETE
# define COMPILER_GCC 1
# if defined(__GNUC_PATCHLEVEL__)
#    define __GNUC_VERSION__ (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
# else
#    define __GNUC_VERSION__ (__GNUC__ * 10000 + __GNUC_MINOR__ * 100)
# endif
# define COMPILER_NAME contact(GCC C++ Compiler (GCC) Version, __GNUC_VERSION__ )
#else
# error Unregistered Compiler
#endif 

#if defined(ARCH_AMD64) || defined(ARCH_ARM64)
#define _64BITBuild 1
#else
#define _64BITBuild 0
#endif

// Zero Fill missing context macros
#if !defined(COMPILER_CL)
# define COMPILER_CL 0
#endif
#if !defined(COMPILER_GCC)
# define COMPILER_GCC 0
#endif
#if !defined(COMPILER_CLANG)
# define COMPILER_CLANG 0
#endif

#if !defined(COMPILER_NAME)
# define COMPILER_NAME "Unknown Compiler"
#endif

//~ Operating System
// TODO(Husam Dababneh): Check if can merge this with the above
#if defined(__linux__)
# define OS_LINUX 1
# define OS_NAME "Linux"
# define OS OS_NAME
#elif defined(_WIN64)
# define OS_WINDOWS 1
# define OS_WINDOWS_64 1
# define OS_NAME "Windows x64"
# define OS OS_NAME
#elif defined(_WIN32)
# define OS_WINDOWS 1
# define OS_WINDOWS_32 1
# define OS_NAME "Windows x32"
# define OS OS_NAME
#elif defined(__unix__)
# define OS_UNIX 1
# define OS_NAME "Unix"
# define OS OS_NAME
#elif defined(__FreeBSD__)
# define OS_FREEBSD 1
# define OS_NAME "FreeBSD"
# define OS OS_NAME
#elif defined(__APPLE__ ) && defined(__MACH__)
# define OS_MAC 1
# define OS_NAME "Apple Macintosh (MAC)"
# define OS OS_NAME
#else 
# error Unregistered Operating System 
#endif 

// Zero Fill missing OS macros
#if !defined(OS_WINDOWS)
# define OS_WINDOWS 0
#endif
#if !defined(OS_WINDOWS_32)
# define OS_WINDOWS_32 0
#endif
#if !defined(OS_WINDOWS_64)
# define OS_WINDOWS_64 0
#endif
#if !defined(OS_LINUX)
# define OS_LINUX 0
#endif
#if !defined(OS_UNIX)
# define OS_UNIX  0
#endif
#if !defined(OS_FREEBSD)
# define OS_FREEBSD 0
#endif
#if !defined(OS_MAC )
# define OS_MAC 0
#endif
#if !defined(OS_NAME)
# define OS_NAME "Unknown Operating System"
#endif

#if !defined(OS)
#error Could not detect Operating System (You may diable this error if you know what you are doing)
#endif

//~ Basic Types
#if COMPILER_CL == 1
typedef  __int8   s8;
typedef  __int16  s16;
typedef  __int32  s32;
typedef  __int64  s64;

typedef  unsigned __int8   u8;
typedef  unsigned __int16  u16;
typedef  unsigned __int32  u32;
typedef  unsigned __int64  u64;

#elif COMPILER_CLANG
typedef  char   s8;
typedef  short  s16;
typedef  int  s32;
typedef  long long  s64;

typedef  unsigned char   u8;
typedef  unsigned short  u16;
typedef  unsigned int    u32;
typedef  unsigned long long  u64;

#else
#error "What The Hell"
#endif // COMPILER_CL == 1

typedef  u8   b8;
typedef  u16  b16;
typedef  u32  b32;
typedef  u64  b64;

typedef u8  Padding8;
typedef u16 Padding16;
typedef u32 Padding32;
typedef u64 Padding64;

typedef  float   f32;
typedef  double   f64;


// NOTE: IF YOU GET "INVALID SUBSCRIPT" ERROR THIS MEANS YOU CANNOT USE THESE TYPES
// NOTE: Can use this if we want to go back to the following
//         "typedef char u8_size_check[(sizeof(u8) == 1) ? 1 : -1];"
// #define StaticAssert(c,l) typedef U8 Glue(l,__LINE__) [(c)?1:-1]


static_assert(sizeof(s8)  == 1, "s8 size must be 1 byte");
static_assert(sizeof(s16) == 2, "s16 size must be 2 bytes");
static_assert(sizeof(s32) == 4, "s32 size must be 4 bytes");
static_assert(sizeof(s64) == 8, "s64 size must be 8 bytes");

static_assert(sizeof(u8)  == 1, "u8 size must be 1 byte");
static_assert(sizeof(u16) == 2, "u16 size must be 2 bytes");
static_assert(sizeof(u32) == 4, "u32 size must be 4 bytes");
static_assert(sizeof(u64) == 8, "u64 size must be 8 bytes");

static_assert(sizeof(b8)  == 1, "b8 size must be 1 byte");
static_assert(sizeof(b16) == 2, "b16 size must be 2 bytes");
static_assert(sizeof(b32) == 4, "b32 size must be 4 bytes");
static_assert(sizeof(b64) == 8, "b64 size must be 8 bytes");

static_assert(sizeof(f32) == 4, "f32 size must be 4 bytes");
static_assert(sizeof(f64) == 8, "f64 size must be 8 bytes");

static_assert(sizeof(Padding8)   == 1, "Padding8 size must be 1 byte");
static_assert(sizeof(Padding16)  == 2, "Padding16 size must be 2 bytes");
static_assert(sizeof(Padding32)  == 4, "Padding32 size must be 4 bytes");
static_assert(sizeof(Padding64)  == 8, "Padding64 size must be 8 bytes");


constexpr  s8  S8_MIN = -0x80;
constexpr s16 S16_MIN = -0x8000;
constexpr s32 S32_MIN = -2'147'483'648; //0x7FFFFFFF; ((1<<31)-1)
constexpr s64 S64_MIN = 0x8000'0000'0000'0000LL;// 0x8000'0000'0000'0000 ((1<<63) - 1)

constexpr  s8  S8_MAX = 0x7F;
constexpr s16 S16_MAX = 0x7FFF;
constexpr s32 S32_MAX = 0x7FFF'FFFF;
constexpr s64 S64_MAX = 0x7FFF'FFFF'FFFF'FFFF;

constexpr  u8  U8_MAX = 0xFF;
constexpr u16 U16_MAX = 0xFFFFL;
constexpr u32 U32_MAX = 0xFFFF'FFFFUL;
constexpr u64 U64_MAX = 0xFFFF'FFFF'FFFF'FFFFULL;


// TODO: Re-think this to constexpr procedures ? 
union FloatUnion {
    u32 i;
    f32 f;
};

union DoubleUnion {
    u64 i;
    f64 f;
};

// Define global constexpr infinity using the union
constexpr FloatUnion PZeroF32   = { 0x0000'0000 };
constexpr FloatUnion NZeroF32   = { 0x8000'0000 };
constexpr FloatUnion PInfF32    = { 0x7F80'0000 };
constexpr FloatUnion NInfF32    = { 0xFF80'0000 };
constexpr FloatUnion NaNF32     = { 0x7FC0'0000 };
constexpr FloatUnion EpsilonF32 = { 0x0000'0001 };
constexpr u32 SignFlipper32     = 0x7FFF'FFFF;


constexpr DoubleUnion PZeroF64   = { 0x0000'0000'0000'0000 };
constexpr DoubleUnion NZeroF64   = { 0x8000'0000'0000'0000 };
constexpr DoubleUnion PInfF64    = { 0x7FF0'0000'0000'0000 };
constexpr DoubleUnion NInfF64    = { 0xFFF0'0000'0000'0000 };
constexpr DoubleUnion NaNF64     = { 0x7FF8'0000'0000'0000 };
constexpr DoubleUnion EpsilonF64 = { 0x0000'0000'0000'0001 };
constexpr u64 SignFlipper64      = 0x7FFF'FFFF'FFFF'FFFF;

// Pi
constexpr f32 PI_F32               = 3.14159265358979323846264338327950288419716939937510f;
constexpr f64 PI_F64               = 3.14159265358979323846264338327950288419716939937510;
// TAU
constexpr f32 TAU_F32              = 6.28318530717958647692528676655900576839433879875020f;
constexpr f64 TAU_F64              = 6.28318530717958647692528676655900576839433879875020; 
// Euler's Number (e)
constexpr f32 E_F32                = 2.71828182845904523536028747135266249775724709369995f; 
constexpr f64 E_F64                = 2.71828182845904523536028747135266249775724709369995; 
// The Golden Ratio (phi)
constexpr f32 PHI_F32              = 1.61803398874989484820458683436563811790028449745619f; 
constexpr f64 PHI_F64              = 1.61803398874989484820458683436563811790028449745619; 
// Square Root of 2
constexpr f32 SQRT2_F32            = 1.41421356237309504880168872420969807856967187537695f; 
constexpr f64 SQRT2_F64            = 1.41421356237309504880168872420969807856967187537695; 
// Square Root of 3
constexpr f32 SQRT3_F32            = 1.73205080756887729352744634150587236694280525381038f; 
constexpr f64 SQRT3_F64            = 1.73205080756887729352744634150587236694280525381038; 
// Natural Logarithm of 2 (ln(2))
constexpr f32 LN2_F32              = 0.69314718055994530941723212145817656807550013436026f; 
constexpr f64 LN2_F64              = 0.69314718055994530941723212145817656807550013436026; 
// Natural Logarithm of 10 (ln(10))
constexpr f32 LN10_F32             = 2.30258509299404568401799145468436420760110148862838f; 
constexpr f64 LN10_F64             = 2.30258509299404568401799145468436420760110148862838; 
// Logarithm of 2 (log2)
constexpr f32 LOG2_F32             = 0.30102999566398119521373889472449302676818988146211f; 
constexpr f64 LOG2_F64             = 0.30102999566398119521373889472449302676818988146211; 
// Logarithm of 10 (log10)
constexpr f32 LOG10_F32            = 0.43429448190325182765112891891660508229439731402818f; 
constexpr f64 LOG10_F64            = 0.43429448190325182765112891891660508229439731402818; 
// The Reciprocal of Pi (1/PI)
constexpr f32 INV_PI_F32           = 0.31830988618301920142441476523210401926754722612628f; 
constexpr f64 INV_PI_F64           = 0.31830988618301920142441476523210401926754722612628; 
// The Reciprocal of the Golden Ratio (1/phi)
constexpr f32 INV_PHI_F32          = 0.61803398874989484820458683436563811790028449745619f; 
constexpr f64 INV_PHI_F64          = 0.61803398874989484820458683436563811790028449745619; 
// Catalan's Constant (used in combinatorics and number theory)
constexpr f32 CATALAN_F32          = 0.91596559417721901505460351493238411077427703814074f; 
constexpr f64 CATALAN_F64          = 0.91596559417721901505460351493238411077427703814074; 
// Apery's Constant (related to zeta function)
constexpr f32 APERY_F32            = 1.20205690315959428539973816151144999076498629234325f; 
constexpr f64 APERY_F64            = 1.20205690315959428539973816151144999076498629234325; 
// The Feigenbaum Constants (Delta and Alpha) - Related to chaos theory
constexpr f32 FEIGENBAUM_DELTA_F32 = 4.66920160910299067185320382046620161513057713120657f; 
constexpr f32 FEIGENBAUM_ALPHA_F32 = 2.50290787509589281635872615861740320234738471124922f; 
constexpr f64 FEIGENBAUM_DELTA_F64 = 4.66920160910299067185320382046620161513057713120657; 
constexpr f64 FEIGENBAUM_ALPHA_F64 = 2.50290787509589281635872615861740320234738471124922; 
// Apéry's constant (zeta(3))
constexpr f32 ZETA3_F32            = 1.20205690315959428539973816151144999076498629234325f; 
constexpr f64 ZETA3_F64            = 1.20205690315959428539973816151144999076498629234325; 
// Euler-Mascheroni Constant
constexpr f32 GAMMA_F32            = 0.57721566490153286060651208999138015227160490432413f; 
constexpr f64 GAMMA_F64            = 0.57721566490153286060651208999138015227160490432413; 


#define NULL 0
#define BYTE(x) (u64)1
#define KB(x)   (u64)(   x  * 1024)
#define MB(x)   (u64)(KB(x) * 1024)
#define GB(x)   (u64)(MB(x) * 1024)
#define TB(x)   (u64)(GB(x) * 1024)

typedef struct String8{
  u8 *str;
  u64 size;
} String8;

typedef struct String16{
  u16 *str;
  u64 size;
} String16;

typedef struct String32{
  u32 *str;
  u64 size;
} String32;

typedef struct String8Node{
  struct String8Node *next;
  String8 string;
} String8Node;

typedef struct String8List{
  String8Node *first;
  String8Node *last;
  u64 node_count;
  u64 total_size;
} String8List;

typedef struct StringJoin{
  String8 pre;
  String8 mid;
  String8 post;
} StringJoin;

enum StringMatchFlags{
  StringMatchFlag_NoCase = 1 << 0,
};

#define str8_expand(s) (int)((s).size), ((s).str)


constexpr b8 ClCompiler            = COMPILER_CL;
constexpr b8 ClangCompiler         = COMPILER_CLANG;
constexpr b8 GccCompiler           = COMPILER_GCC;
constexpr b8 is64Bit               = _64BITBuild;
constexpr b8 is32Bit               = !is64Bit;
constexpr const char* CompilerName = COMPILER_NAME;
constexpr const char* OSName       = OS_NAME;
constexpr const char* CPUArchitecture = ARCH_NAME;

enum Month {
    Month_January = 1,
    Month_February,
    Month_March,
    Month_April,
    Month_May,
    Month_June,
    Month_July,
    Month_August,
    Month_September,
    Month_October,
    Month_November,
    Month_December,
};

enum WeekDay {
    WeekDay_Sunday = 1,
    WeekDay_Monday,
    WeekDay_Tuesday,
    WeekDay_Wednesday,
    WeekDay_Thursday,
    WeekDay_Friday,
    WeekDay_Saturday
};

function String8 stringFromMonth(Month);

//~ forward declarations
function f32 abs_f32(f32);
function f64 abs_f64(f64);




#endif // BASE_H


