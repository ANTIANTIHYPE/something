@echo off
setlocal ENABLEDELAYEDEXPANSION

set tests[0]=aes
set tests[1]=genius_ptr
set tests[2]=bitset
set tests[3]=coroutine
set tests[4]=endianness
set tests[5]=qa

set count=5

set "name=build"

if not exist %name% mkdir %name%
cd %name%

for /L %%i in (0,1,%count%) do (
    set "file=!tests[%%i]!"
    echo Compiling !file!.cpp...
    g++ -o !file! ..\!file!.cpp --std=c++2b -O3 -I..\..
)

if %errorlevel%==1 pause

endlocal