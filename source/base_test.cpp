/* ========================================================================
   $File: base_test.cpp
   $Date: 2025-03-02
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   $Description: 
    ========================================================================*/


#include "stdio.h"
#include "base.cpp"


struct TestStruct {
    u32 a;
    Padding32 padding;
    f64 b;
    u8 c[10];
    Padding16 paddgin2;
    Padding32 paddgin3;
};

int main() {

    printf("-------------------------------\n");
    printf("Compile Time Info (these are given by the compiler)\n");
    printf("Compiler Name   : %s\n", CompilerName);
    printf("Operating System: %s\n", OS_NAME);
    printf("CPU Architecture: %s\n", CPUArchitecture);
    printf("is64            : %s\n", is64Bit ? "true" : "false");
    printf("is32            : %s\n", is32Bit ? "true" : "false");

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
    printf("Offset of member 'b' in TestStruct: %lld\n", offsetof(TestStruct, b));
    printf("Size of member 'b' in TestStruct: %zu\n", member_size(TestStruct, b));
    
    TestStruct ts;
    printf("Count of elements in 'c' array: %llu\n", countof(ts.c));


    printf("----------- Macro Tests 2 -------------\n");
    
    u32 x = 45, b = 8;
    u32 y = 0;
    printf("alignPow2(%d, %d)     = %d\n", x, b, alignPow2(x, b));  
    printf("alignDownPow2(%d, %d) = %d\n", x, b, alignDownPow2(x, b));
    printf("alignPadPow2(%d, %d)  = %d\n", x, b, alignPadPow2(x, b));
    printf("isPow2(%d)            = %s\n", b, bool_value(isPow2(b)));  
    printf("isPow2OrZero(%d)      = %s\n", y, bool_value(isPow2OrZero(y)));   
    y = 16;
    printf("isPow2OrZero(%d)     = %s\n", y, bool_value(isPow2OrZero(y)));
    y = 15;
    printf("isPow2OrZero(%d)     = %s\n", y, bool_value(isPow2OrZero(y)));

    f32 negativeValue32 = -100.f;
    f32 negativeValue64 = -100.0;
    printf("abs_f32(%f)     = %f\n", negativeValue32, abs_f32(negativeValue32));
    printf("abs_f64(%f)     = %f\n", negativeValue64, abs_f64(negativeValue64));

    printf("----------- Functions Tests -------------\n");
    printf("stringFromMonth(%d)     = %s\n", Month_March, stringFromMonth(Month_March).str);
    
    return 0;
}
