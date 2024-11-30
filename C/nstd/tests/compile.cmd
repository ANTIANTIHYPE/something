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
    gcc -o !file! %%f --std=c2x -O1 -Wall -I..\.. || ( @REM -O1 due to hook block with -O2 and higher
        echo Compilation failed for !file!.c
        pause
        exit /b 1
    )
    set /a count+=1
)

if %count%==0 (
    echo No tests found to compile.
) else (
    echo All tests compiled successfully.
)

if %errorlevel%==1 pause && exit /b 1

popd
endlocal
