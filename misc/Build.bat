@echo off

IF NOT EXIST .\bin-int mkdir bin-int
IF NOT EXIST .\bin mkdir bin


set CommonFlags=/Fe:..\bin\main.exe /std:c++17 
set ReleaseFlags=%CommonFlags% /EHsc /MD
set DebugFlags=%CommonFlags% /W2 /Z7 /EHsc /DEBUG:FULL /MDd

set Files=../source/main.cpp
set DLibs=zlibstaticd.lib
set RLibs=zlibstatic.lib


REM the /I after the IF means to do case insensitive compare

[ -z %1 ] && goto Usage

if /I [%1] == [Debug] CALL :Compile "%DebugFlags%" "%DLibs%" & goto :eof
if /I [%1] == [Release] CALL :Compile "%ReleaseFlags%" "%RLibs%" & goto :eof
if /I [%1] == [Clean] goto Clean
if /I [%1] == [Usage] goto Usage

:Usage
echo Usage: Build.exe ^<Debug^|Release^|Clean^>
goto :eof

:Compile
pushd bin-int
echo Entering directory `bin-int'
cl %~1  %Files% %~2  /link /LIBPATH:"..\lib\"
echo Leaving  directory `bin-int'
popd
EXIT %ERRORLEVEL%

:Clean
REM Todo
echo test
goto :eof

:eof
EXIT %ERRORLEVEL%
