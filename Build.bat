@echo off

IF NOT EXIST .\bin-int mkdir bin-int
IF NOT EXIST .\bin mkdir bin


set CommonFlags=/Fo:main_cl.obj  /c /nologo /diagnostics:caret /fastfail /FC /std:c++20 /wd4505
rem set ReleaseFlags=%CommonFlags% /EHsc /MD
set DebugFlags=%CommonFlags% /Wall /Z7 /EHsc /DEBUG:FULL

rem -std=c++20
set ClangCommonFlags=-o main_clang.obj -c -gcodeview -Wall -Wno-c++98-compat-pedantic -Wno-double-promotion -Wno-old-style-cast -Wno-zero-as-null-pointer-constant -Wno-unused-function -Wno-cast-qual
rem set ClangReleaseFlags=%ClangCommonFlags% -O3 -DNDEBUG -fvisibility=hidden
set ClangDebugFlags=%ClangCommonFlags% 
rem /Wall 
rem /g -Wall -Wextra -O0


set Files=../source/base_test.cpp
set DLibs=zlibstaticd.lib
set RLibs=zlibstatic.lib

set ClangFiles=../source/base_test.cpp
set ClangDLibs=zlibstaticd.lib
set ClangRLibs=zlibstatic.lib

rem remove /OPT:NOREF when note testing base layer
set LINKER_OPTIONS=/WX /nologo /DEBUG:FULL /OPT:NOREF /INCREMENTAL:NO
set layouts=
rem /d1reportSingleClassLayoutTestStruct

:Compile
pushd bin-int
echo Entering directory `bin-int'
echo.

rem /link /LIBPATH:"..\lib\"
echo Compileing Using CL (MSVC Compiler)
cl %layouts% %DebugFlags%  %Files%

echo.
echo Compileing Using Clang (Clang-cl Compiler)
clang-cl %ClangDebugFlags% %ClangFiles%

echo.
echo Linking
rem using RADLINK
rem radlink

link main_cl.obj /OUT:..\bin\main_cl.exe %LINKER_OPTIONS%
link main_clang.obj /OUT:..\bin\main_clang.exe %LINKER_OPTIONS%


echo.
echo Leaving  directory `bin-int'
popd
