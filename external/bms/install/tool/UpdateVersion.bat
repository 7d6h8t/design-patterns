@echo off
setlocal EnableDelayedExpansion
set TargetFile=%1
set TargetVersion=%2
set MainVer=
set MajorVer=
set MinorVer=
set BuildCnt=

::1. check exist version file
set TempFile=%TargetFile%.tmp
if not exist %TargetFile% (
    set ErrorString="TargetFile is Empty"
    goto :Error
)

::2. check Argument
if "%TargetVersion%"=="" (
    set ErrorString="Using VersionUpdater.bat *.inc version"
    goto :Error
)

::3. parsing version String
call :parseVersionStr %TargetVersion%
if not %ERRORLEVEL%==0 (
    set ErrorString="Paring Version String Error"
    goto :Error
)

::4. Delete TempFile
if exist %TempFile% (
    del "%TempFile%" /s /f /q
)

set charSpace= 
set charTap=	
::5. UpdateVersion To TempFile
for /f "tokens=*" %%A in (%1) do call :processline %%A

::6. Rename File Temp->Target
call :renameFile "%TempFile%" "%TargetFile%"
goto :eof

::Parsing version from Line 
:processline
set lineText=%*
for /F "tokens=1,2,3 delims=%charTap%%charSpace%" %%a in ("%lineText%") do call :inRoof %%a %%b %%c
::Write TempFile
>>"%TempFile%" echo %lineText%
goto :eof

:inRoof
::Update version String
set bUpdate=false
set OrgVersion=%3
if "%1"=="#define" (
    if "%2"=="VERSION_MAIN" (
        set bUpdate=true
        set UpdateVersion=%MainVer%       
    )
    if "%2"=="VERSION_MAJOR" (
        set bUpdate=true
        set UpdateVersion=%MajorVer%
    )
    if "%2"=="VERSION_MINOR" (
        set bUpdate=true
        set UpdateVersion=%MinorVer%
    )
    if "%2"=="VERSION_BUILDCNT" (
        set bUpdate=true
        set UpdateVersion=%BuildCnt%
    )
)

if "%bUpdate%"=="true" (
    call :replaceText "%lineText%" %OrgVersion% %UpdateVersion%
)

goto :eof

::Increase Version
:replaceText
set OrgText=%1
set Old=%2
set New=%3

call set lineText=%%OrgText:!Old!=!New!%%
call set lineText=%%lineText:"=%%

goto :eof

:: TempFile To TargetFile
:renameFile
set OrgPath=%1
set TargetPath=%2
call del %TargetPath% /s /f /q
echo %OrgPath%
echo %TargetPath%

xcopy %OrgPath% %TargetPath%* ^
/y

call del %OrgPath% /s /f /q
goto :eof

::token version String
:parseVersionStr
set VersionStr=%1
for /F "tokens=1,2,3,4 delims=." %%a in ("%VersionStr%") do (
    set MainVer=%%a
    set MajorVer=%%b
    set MinorVer=%%c
    set BuildCnt=%%d
)

call :validString %MainVer%
if not %ERRORLEVEL%==0 (
    goto :Error
)

call :validString %MajorVer%
if not %ERRORLEVEL%==0 (
    goto :Error
)

call :validString %MinorVer%
if not %ERRORLEVEL%==0 (
    goto :Error
)

call :validString %BuildCnt%
if not %ERRORLEVEL%==0 (
    goto :Error
)

echo %MainVer%.%MajorVer%.%MinorVer%.%BuildCnt%
goto :eof

:validString
set targetStr=%1
::check empty
if "%targetStr%"=="" (
    echo "Error Using version Main.Major.Minor.BuildCnt"    
    exit 1
)
::check strlen
call :strLen targetStr strlen
if %strLen% GTR 3 (
	echo "Version Max is 3"
    exit 1
)

::check atoi
echo %targetStr%| findstr /r "^[0-9]*$">nul
if not %errorlevel%==0 (
    echo "Fail atoi"   
    exit 1
)
goto :eof

:strLen
setlocal enabledelayedexpansion

:strLen_Loop
   if not "!%1:~%len%!"=="" set /A len+=1 & goto :strLen_Loop
(endlocal & set %2=%len%)
goto :eof

:Error
echo "Error %ErrorString%"
set ERRORLEVEL=1

:eof

endlocal