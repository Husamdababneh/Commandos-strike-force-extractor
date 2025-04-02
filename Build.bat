@echo off

IF NOT EXIST .\bin-int mkdir bin-int
IF NOT EXIST .\bin mkdir bin


set MSVCFlags=/nologo /diagnostics:caret /fastfail /c /std:c++20 /DNO_CRT /Wall /D_CRT_NO_VA_START_VALIDATION
rem /showIncludes
set CommonFlags=/Fo:main_cl.obj /Zi /FC /Gm- /GR- /GS- /EHa- /Oi /EHs- /EHc- /GF /Gs16777216
set DebugFlags=%CommonFlags% /DEBUG:FULL /wd4505 /wd5039 /wd4042 /wd4668 /wd4576 /wd4189

rem -Zi -nologo -Gm- -GR- -EHa- -Oi -GS- -Gs9999999
rem /d1reportSingleClassLayoutArena
rem 

rem -std=c++20
set ClangSuppressedWarns=-Wno-double-promotion -Wno-old-style-cast -Wno-zero-as-null-pointer-constant -Wno-unused-function -Wno-cast-qual -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-covered-switch-default -Wno-unused-macros -Wno-nonportable-system-include-path
set ClangCommonFlags=/o main_clang.obj /c -gcodeview -Od -Wall -Wextra /std:c++20 /TP   %ClangSuppressedWarns%
set ClangDebugFlags=%ClangCommonFlags% 
rem /g -Wall -Wextra -O0 /showIncludes


set Files=../source/base_test.cpp
set DLibs=zlibstaticd.lib
set RLibs=zlibstatic.lib

set ClangFiles=../source/base_test.cpp
set ClangDLibs=zlibstaticd.lib
set ClangRLibs=zlibstatic.lib

rem remove /OPT:NOREF when note testing base layer
set LINKER_OPTIONS=/WX /nologo /DEBUG:FULL /OPT:NOREF /STACK:0x1000000,0x1000000 /INCREMENTAL:NO /SUBSYSTEM:CONSOLE /NODEFAULTLIB /ENTRY:main Kernel32.lib
rem 

:Compile
pushd bin-int
echo Entering directory `bin-int'
echo.

echo Compileing Using CL (MSVC Compiler)
rem ml64 /nologo -Zi /Fo chstk.obj /c ../source/chkstk.asm
cl %MSVCFlags% %layouts% %DebugFlags%  %Files%

rem echo.
rem echo Compileing Using Clang (Clang-cl Compiler)
rem echo clang-cl %ClangDebugFlags% %ClangFiles%
rem clang-cl %ClangDebugFlags% %ClangFiles%

echo.
echo Linking
rem using RADLINK
rem radlink
rem chstk.obj
link main_cl.obj  /OUT:..\bin\main_cl.exe %LINKER_OPTIONS% 
rem link main_clang.obj /OUT:..\bin\main_clang.exe %LINKER_OPTIONS%


echo.
echo Leaving  directory `bin-int'
popd
