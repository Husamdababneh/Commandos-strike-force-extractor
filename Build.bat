@echo off

IF NOT EXIST .\bin-int mkdir bin-int
IF NOT EXIST .\bin mkdir bin


set CommonFlags=/Fo:main_cl.obj  /c /nologo /diagnostics:caret /fastfail /FC /std:c++20 /wd4505 /wd5039 /wd4042 /wd4668
set DebugFlags=%CommonFlags% /Wall /Z7 /EHs- /EHc- /DEBUG:FULL  /DNO_CRT -GS- /d1reportSingleClassLayoutFormat 
rem /showIncludes

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
set LINKER_OPTIONS=/WX /nologo /DEBUG:FULL /OPT:NOREF /INCREMENTAL:NO /SUBSYSTEM:CONSOLE /NODEFAULTLIB /ENTRY:main Kernel32.lib
rem 

:Compile
pushd bin-int
echo Entering directory `bin-int'
echo.

echo Compileing Using CL (MSVC Compiler)
cl %layouts% %DebugFlags%  %Files%

rem echo.
rem echo Compileing Using Clang (Clang-cl Compiler)
rem echo clang-cl %ClangDebugFlags% %ClangFiles%
rem clang-cl %ClangDebugFlags% %ClangFiles%

echo.
echo Linking
rem using RADLINK
rem radlink

link main_cl.obj /OUT:..\bin\main_cl.exe %LINKER_OPTIONS% 
rem link main_clang.obj /OUT:..\bin\main_clang.exe %LINKER_OPTIONS%


echo.
echo Leaving  directory `bin-int'
popd
