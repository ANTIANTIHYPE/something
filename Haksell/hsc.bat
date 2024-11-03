@echo off
color 3
title Haskell Compiler
setlocal enabledelayedexpansion

set downloaded=0
set cdownloaded=0
set "ghc="

echo Checking for GHCup...
reg query "HKEY_USERS\S-1-5-21-1917770473-3348344261-3804054489-1000\Environment" >nul 2>&1
if %errorlevel% equ 0 (
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
    echo GHCup not installed
)

if "!downloaded!"=="0" (
    title GHCup setup && color a && echo Downloading GHCup && cls
    powershell -Command "Set-ExecutionPolicy Bypass -Scope Process -Force;[System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; try { Invoke-Command -ScriptBlock ([ScriptBlock]::Create((Invoke-WebRequest https://www.haskell.org/ghcup/sh/bootstrap-haskell.ps1 -UseBasicParsing))) -ArgumentList $true } catch { Write-Error $_ }"
    ping 127.0.0.1 -n 4 > nul && cls
) else (
    echo Skipping GHCup setup...
    ping 127.0.0.1 -n 2 > nul && cls
)

if "!cdownloaded!"=="0" (
    title cabal setup && color a && Downloading cabal && cls
    %ghc%\ghcup\ghcup.exe install --set cabal latest
)

color 3
set count=1
for %%f in (*.hs) do (
    echo !count!. %%f
    set file!count!=%%f
    set /a count+=1
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

set folder=compiled_hs
if not exist %folder% mkdir %folder%
set temp_folder=%folder%\Temp\%chars%
if not exist %temp_folder% mkdir %temp_folder%
set exe_path=%folder%\%name%.exe
set PATH=%ghc%ghcup\bin;%PATH%

set "tmp=%TEMP%"
set "zfp=%tmp%\downloaded.zip"
set "output=%tmp%\hsc_temp"

rem echo @echo off> c.bat
rem echo &rem setlocal> c.bat
rem echo set "library_name=%1"> c.bat

rem echo set downloaded=0> c.bat
rem echo set "ghc="> c.bat

rem echo echo Checking for GHCup...> c.bat
rem echo reg query "HKEY_USERS\S-1-5-21-1917770473-3348344261-3804054489-1000\Environment" >nul 2>&1> c.bat
rem echo if %errorlevel% equ 0 (> c.bat
rem echo    for /f "tokens=3" %%A in ('reg query "HKEY_USERS\S-1-5-21-1917770473-3348344261-3804054489-1000\Environment" /v "GHCUP_INSTALL_BASE_PREFIX" 2^>nul') do (> c.bat
rem echo        if not "%%A"=="" (> c.bat
rem echo            echo GHCup found:                              %%A> c.bat
rem echo            set downloaded=1> c.bat
rem echo            set ghc=%%A> c.bat
rem echo        ) else (> c.bat
rem echo            echo GHCup registry is nil ?> c.bat
rem echo        )> c.bat
rem echo    )> c.bat
rem echo )> c.bat
rem echo.> c.bat
rem echo %ghc%"%ghc%\ghcup\bin\cabal.exe" list --installed %library_name% | findstr /c:"%ghc%\cabal\packages\hackage.haskell.org:%library_name%" >nul> c.bat
rem echo.> c.bat
rem echo if %errorlevel% equ 0 (> c.bat
rem echo    echo Found %library_name% library, skipping set up> c.bat
rem echo ) else (> c.bat
rem echo    echo 0> c.bat
rem echo    echo Installing %library_name% library...> c.bat
rem echo    echo cabal update && cabal install --lib %library_name%> c.bat
rem echo )> c.bat
rem echo.> c.bat
rem echo &rem endlocal> c.bat
rem pause

if exist "%output%" (
    rmdir /s /q "%output%"
)
mkdir "%output%"
rem pause
move /y "./c.bat" "%output%"
set "main=%output%\c.bat"
if exist "%main%" (
    start "" "%main%"
) else (
    echo Uh what...?
)

ghc -o %exe_path% -outputdir %temp_folder% %name% && cls && title %name% && %exe_path%
if %errorlevel% == 1 pause
