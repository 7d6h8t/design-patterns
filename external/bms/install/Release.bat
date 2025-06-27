@echo off
setlocal enableextensions enabledelayedexpansion

TITLE Release BMS...

::set Path
set OrgPath=%CD%

for %%i in ("%~dp0/..") do set "SolutionPath=%%~fi"

::Check visual studio Path
cd /d "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\"
for /F "tokens=* USEBACKQ" %%F IN (`vswhere.exe -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) DO (
    set msbuildpath=%%F
)
if "%msbuildpath%"=="" (
    set ErrorString="Check Visual Studio Setup"
    GOTO ERROR
)

::Check InnoSetup Path
for /f "tokens=1,2*" %%a in ('reg query "HKLM\SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall\Inno Setup 6_is1" /v "InstallLocation" 2^>nul') do set "InnoSetupDir=%%c"
if "%InnoSetupDir%"=="" (
    set ErrorString="Check InnoSetup 6.0 Setup"
    GOTO ERROR
)

set InnoSetupISCC=%InnoSetupDir%\ISCC.exe

:: Store start time
FOR /f "tokens=1-4 delims=:.," %%T IN ("%TIME%") DO (
	SET StartTIME=%TIME%
	:: Fix leading zero problem
	SET /a Start100S=%%T*360000+1%%U*6000+1%%V*100+1%%W - 610100
)

::Build Solutions Rebuild
echo Start Building external
echo %SolutionPath%\build\external.sln
"%msbuildpath%" /nologo "%SolutionPath%\build\external.sln" /m /t:Rebuild "/p:Configuration=Release;Platform=x64"
if %ERRORLEVEL% NEQ 0 (
    set ErrorString="external.sln Build Error"
    GOTO ERROR
)

echo Start Building bms
"%msbuildpath%" /nologo "%SolutionPath%\build\bms.sln" /m /t:Rebuild "/p:Configuration=Release;Platform=x64"
if %ERRORLEVEL% NEQ 0 (
    set ErrorString="bms.sln Build Error"
    GOTO ERROR
)
cd /d "%ORIGPATH%"

::Make SetupFile
echo start making setup
"%InnoSetupISCC%" "%SolutionPath%\install\BMS_setup.iss"
if %ERRORLEVEL% NEQ 0 (
    set ErrorString="standard setup build Error"
    GOTO ERROR
)

:: Retrieve Stop time
FOR /f "tokens=1-4 delims=:.," %%T IN ("%TIME%") DO (
	SET StopTIME=%TIME%
	:: Fix leading zero problem
	SET /a Stop100S=%%T*360000+1%%U*6000+1%%V*100+1%%W - 610100
)

:: Show Build Time
if %Stop100S% LSS %Start100S% set /a Stop100S+=8640000
set /a TookTimeSec=(%Stop100S%-%Start100S%)/100
set /a TookTimeMin=TookTimeSec/60
set /a TookTimeSec=%TookTimeSec%-%TookTimeMin%*60

echo Started: %StartTime%
echo Stopped: %StopTime%
echo Elapsed: %TookTimeMin% min. %TookTimeSec% sec.

PAUSE
GOTO :END

:ERROR
echo "Fail Release BMS : %ErrorString%"
pause
exit /b 1
:END
exit /b 0
endlocal