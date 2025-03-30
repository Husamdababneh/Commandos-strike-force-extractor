/* ========================================================================
   $File: base_test.cpp
   $Date: 2025-03-02
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   $Description: 
    ========================================================================*/

#include "base.cpp"
#include "base_platform.cpp"
#include "base_arena.cpp"
#include "base_printf.cpp"

void testBaseLayer();

int main() {
    
    Arena* arena = arena_alloc_(&defaultArenaParams);

    u8* buffer = push_array(arena, u8, 256); 
    auto sss = "[This Is My String]"_s8;
    u64 integer = 321;
    auto a = format(-123.f);
    u64 stringSize = printf(buffer, 256,
                            R"str(
This is a string[%]
This is integer variable[%]
This is integer literal[%]
This is float32 literal[%]
This is float32 literal[%]
This is float32 literal[%]
)str"
                            ""_s8,
                            format("String"),
                            format(integer),
                            format(123),
                            format(NInfF32.f),
                            format(NaNF32.f),
                            format(123.300f));
    
    // auto floatFormat = format(123.f);
    auto floatFormat = format(123.0);

    HANDLE stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    BOOL success = WriteFile(stdout,
                             buffer,
                             (u32)stringSize, // TODO: 
                             NULL,
                             NULL);
    (void)success;
    
    
    #if 0
    size size = GB(1);

    vptr memory = VirtualAlloc(NULL,
                               size, 
                               MEM_COMMIT,
                               PAGE_READWRITE);

    if (!memory) {
        printf("Error: could not alloc memmory\n");
        return -1;
    }


    BOOL success = VirtualFree(memory,
                               0,
                               MEM_RELEASE);

    if (!success) {
        printf("Error: could not free memmory... exiting anyways\n");
    }
    #endif
    return 0;
}

#if 0
void testBaseLayer() {

    struct TestStruct {
        u32 a;
        Padding32 padding;
        f64 b;
        u8 c[10];
        Padding16 paddgin2;
        Padding32 paddgin3;
    };
    
    printf("-------------------------------\n");
    printf("Compile Time Info (these are given by the compiler)\n");
    printf("Compiler Name   : %s\n", CompilerName);
    printf("Operating System: %s\n", OS_NAME);
    printf("CPU Architecture: %s\n", CPUArchitecture);
    printf("is64            : %s\n", is64Bit ? "true" : "false");
    printf("is32            : %s\n", is32Bit ? "true" : "false");
    printf("C++ Version     : %ld\n", __cplusplus);
    
    printf("-------------------------------\n");
    printf("S8_MIN  = %d\n",    S8_MIN);
    printf("S16_MIN = %hd\n",  S16_MIN);
    printf("S32_MIN = %d\n",  S32_MIN);
    printf("S64_MIN = %lld\n", S64_MIN);
    
    printf("S8_MAX  = % 1d\n",    S8_MAX);
    printf("S16_MAX = % 1hd\n",  S16_MAX);
    printf("S32_MAX = % 1d\n",  S32_MAX);
    printf("S64_MAX = % 1lld\n", S64_MAX);
    
    printf("U8_MAX  =  %1u\n",    U8_MAX);
    printf("U16_MAX =  %1hu\n",  U16_MAX);
    printf("U32_MAX =  %1u\n",  U32_MAX);
    printf("U64_MAX =  %1llu\n", U64_MAX);
    
    printf("--------- IEEE 754 32-bit ---------\n");
    printf("PI                = % 1E\n", PI_F32);
    printf("NaN               = % 1E\n", NaNF32.f);
    printf("Positive Infinity = % 1E\n", PInfF32.f);      
    printf("Negative Infinity = % 1E\n", NInfF32.f);
    printf("Positive Zero     = % 1E\n", PZeroF32.f);
    printf("Negative Zero     = % 1E\n", NZeroF32.f);
    printf("Epsilon           = % 1E\n", EpsilonF32.f);
    
    printf("--------- IEEE 754 64-bit ---------\n");   
    printf("PI                = % 1E\n", PI_F64);
    printf("NaN               = % 1E\n", NaNF64.f);
    printf("Positive Infinity = % 1E\n", PInfF64.f);
    printf("Negative Infinity = % 1E\n", NInfF64.f);
    printf("Positive Zero     = % 1E\n", PZeroF64.f);
    printf("Negative Zero     = % 1E\n", NZeroF64.f);
    printf("Epsilon           = % 1E\n", EpsilonF64.f);
    
    printf("----------- Macro Tests -------------\n");
    printf(R""""(struct TestStruct {
           u32 a;
           f64 b;
           u8 c[10];
       };
       )"""");
    printf("Offset of member 'b' in TestStruct: %llu\n", offsetof(TestStruct, b));
    printf("Size of member 'b' in TestStruct: %zu\n", member_size(TestStruct, b));
    
    TestStruct ts;
    printf("Count of elements in 'c' array: %llu\n", countof(ts.c));
    
    
    printf("----------- Macro Tests 2 -------------\n");
    
    u32 x = 45, b = 8;
    u32 y = 0;
    printf("alignPow2(%u, %u)     = %u\n", x, b, alignPow2(x, b));  
    printf("alignDownPow2(%u, %u) = %u\n", x, b, alignDownPow2(x, b));
    printf("alignPadPow2(%u, %u)  = %u\n", x, b, alignPadPow2(x, b));
    printf("isPow2(%u)            = %s\n", b, bool_value(isPow2(b)));  
    printf("isPow2OrZero(%u)      = %s\n", y, bool_value(isPow2OrZero(y)));   
    y = 16;
    printf("isPow2OrZero(%u)     = %s\n", y, bool_value(isPow2OrZero(y)));
    y = 15;
    printf("isPow2OrZero(%u)     = %s\n", y, bool_value(isPow2OrZero(y)));
    
    f32 negativeValue32 = -100.f;
    f32 negativeValue64 = -100.0;
    printf("abs_f32(%f)     = %f\n", negativeValue32, abs_f32(negativeValue32));
    printf("abs_f64(%f)     = %f\n", negativeValue64, abs_f64(negativeValue64));
}
#endif
