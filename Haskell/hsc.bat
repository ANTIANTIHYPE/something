@echo off
color 3
title Haskell compiler
setlocal enabledelayedexpansion

set downloaded=0
set cdownloaded=0
set "ghc="

echo Checking for GHCup...
reg query "HKEY_USERS\S-1-5-21-1917770473-3348344261-3804054489-1000\Environment" >nul 2>&1
if %errorlevel%==0 (
    for /f "tokens=3" %%A in ('reg query "HKEY_USERS\S-1-5-21-1917770473-3348344261-3804054489-1000\Environment" /v "GHCUP_INSTALL_BASE_PREFIX" 2^>nul') do (
        if not "%%A"=="" (
            echo GHCup found:                              %%A
            set downloaded=1
            set ghc=%%A
        ) else (
            echo GHCup registry is nil ?
        )
    )
    for /f "tokens=3" %%B in ('reg query "HKEY_USERS\S-1-5-21-1917770473-3348344261-3804054489-1000\Environment" /v "CABAL_DIR" 2^>nul') do (
        if not "%%B"=="" (
            echo Cabal found:                              %%B
            set cdownloaded=1
        ) else (
            echo cabal is not installed
        )
    )
) else (
    echo GHCup not installed, installing...
)

if "!downloaded!"=="0" (
    title GHCup setup && color a && cls
    powershell -Command "Set-ExecutionPolicy Bypass -Scope Process -Force;[System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; try { Invoke-Command -ScriptBlock ([ScriptBlock]::Create((Invoke-WebRequest https://www.haskell.org/ghcup/sh/bootstrap-haskell.ps1 -UseBasicParsing))) -ArgumentList $true } catch { Write-Error $_ }"
    ping 127.0.0.1 -n 4>nul && cls
) else (
    echo Skipping GHCup setup...
    ping 127.0.0.1 -n 2>nul && cls
)

if "!cdownloaded!"=="0" (
    title cabal setup && color a && cls
    %ghc%\ghcup\ghcup.exe install --set cabal latest
)

color 3
set count=1
set yes=0
for %%f in (*.hs) do (
    echo !count!. %%f
    set file!count!=%%f
    set /a count+=1
    set yes=1
)

if %yes%==0 (
    echo No files found in the current directory, aborting...
    pause
    goto EOF
)

set "chars="
for /f %%i in ('powershell -Command "[char[]](97..122) | Get-Random -Count 10"') do (
    set "chars=!chars!%%i"
)

set /p choice="Enter the number of the file you want to compile: " && cls
set /p name="Enter the program's name (default: .hs's name): " && cls

if "%name%"=="" (
    set name=!file%choice%!
)

@REM strip
for %%x in (!name!) do set "base_name=%%~nx"

set folder=compiled_hs
if not exist %folder% mkdir %folder%
set temp_folder=%folder%\Temp\%chars%
if not exist %temp_folder% mkdir %temp_folder%
set exe_path=%folder%\%base_name%.exe
set PATH=%ghc%ghcup\bin;%PATH%

set "tmp=%TEMP%"

ghc -o %exe_path% -outputdir %temp_folder% %base_name%
if %errorlevel%==1 pause

cls && title %base_name% && %exe_path%

EOF:

endlocal