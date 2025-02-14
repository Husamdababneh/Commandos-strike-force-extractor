@echo off

IF NOT EXIST .\bin-int mkdir bin-int
IF NOT EXIST .\bin mkdir bin


set CommonFlags=/Fe:..\bin\main.exe /std:c++17  -diagnostics:caret
set ReleaseFlags=%CommonFlags% /EHsc /MD
set DebugFlags=%CommonFlags% /W2 /Z7 /EHsc /DEBUG:FULL
rem /MDd

set Files=../source/main.cpp
set DLibs=zlibstaticd.lib
set RLibs=zlibstatic.lib


REM the /I after the IF means to do case insensitive compare

REM [ -z %1 ] && CALL :Compile "%DebugFlags%" "%DLibs%"  

REM if /I [%1] == [Debug] CALL :Compile "%DebugFlags%" "%DLibs%" & goto :eof
REM if /I [%1] == [Release] CALL :Compile "%ReleaseFlags%" "%RLibs%" goto :eof
REM if /I [%1] == [Clean] goto Clean & goto :eof
REM if /I [%1] == [Usage] goto Usage & goto :eof

REM :Usage
REM echo Usage: Build.exe ^<Debug^|Release^|Clean^>
REM goto :eof

:Compile
pushd bin-int
echo Entering directory `bin-int'
REM cl %~1 %Files% %~2  /link /LIBPATH:"..\lib\"
REM /d1reportSingleClassLayoutTextureInfo
cl %DebugFlags%  %Files% %DLibs%  /link /LIBPATH:"..\lib\"

echo Leaving  directory `bin-int'
popd
EXIT %ERRORLEVEL%
