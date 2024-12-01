@echo off
setlocal ENABLEDELAYEDEXPANSION

set "name=build"

if not exist "%name%" (
    mkdir "%name%" || (
        echo Failed to create directory "%name%"
        pause
        exit /b 1
    )
)

pushd "%name%" || (
    echo Failed to change directory to "%name%"
    pause
    exit /b 1
)

set count=0

for %%f in (..\*.c) do (
    set "file=%%~nf"
    echo Compiling !file!.c...

    set "flags=--std=c2x -Wall -I..\.."

    @REM ??? TODO: fix
    if "!file!"=="Windows++" (
        set "flags=!flags! -O3 -lole32 -loleaut32 -lwmi32 -lwbemuuid"
    ) else if "!file!"=="Hooking" (
        set "flags=!flags! -O1"
    )

    gcc -o !file! %%f !flags! || (
        echo Compilation failed for !file!.c
        pause
        exit /b 1
    )

    set /a count+=1
)

if !count!==0 (
    echo No tests found to compile.
) else (
    echo All tests compiled successfully.
)

popd
endlocal
