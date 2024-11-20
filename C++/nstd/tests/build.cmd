@echo off
setlocal ENABLEDELAYEDEXPANSION

set tests[0]=aes
set tests[1]=genius_ptr
set tests[2]=bitset
set tests[3]=coroutine
set tests[4]=endianness
set tests[5]=qa
set tests[6]=ml
set tests[7]=stupid_ptr

set count=0

:count_tests
if defined tests[%count%] (
    set /a count+=1
    goto count_tests
)

set "name=build"
if not exist "%name%" (
    mkdir "%name%" || (
        echo Failed to create directory "%name%"
        exit /b 1
    )
)

cd "%name%" || (
    echo Failed to change directory to "%name%"
    exit /b 1
)

for /L %%i in (0,1,%count%) do (
    set "file=!tests[%%i]!"
    if defined file (
        echo Compiling !file!.cpp...
        g++ -o !file! ..\!file!.cpp --std=c++2b -O3 -I..\.. || (
            echo Compilation failed for !file!.cpp
            exit /b 1
        )
    )
)

echo All files compiled successfully.
endlocal