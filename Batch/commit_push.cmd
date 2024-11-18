@ECHO OFF
SETLOCAL
SET "DEFAULTBRANCH=master"
SET /p BRANCH="Enter branch (default is %DEFAULTBRANCH%): "
IF "%BRANCH%"=="" SET "BRANCH=%DEFAULTBRANCH%"
SET /p "MSG=Commit message: "
IF NOT "%BRANCH%"=="%DEFAULTBRANCH%" (
   git fetch
   git switch %BRANCH%
)
git add .
git commit -m "%MSG%"
git push -u origin %BRANCH%
IF %ERRORLEVEL% == 1 PAUSE
ENDLOCAL