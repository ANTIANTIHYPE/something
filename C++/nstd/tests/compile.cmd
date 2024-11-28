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

for %%f in (..\*.cpp) do (
    set "file=%%~nf"
    echo Compiling !file!.cpp...
    g++ -o !file! %%f --std=c++2b -O3 -Wall -I..\.. || (
        echo Compilation failed for !file!.cpp
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

if %errorlevel%==1 pause

popd
endlocal
