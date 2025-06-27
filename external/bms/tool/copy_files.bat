@echo off
setlocal enableextensions
:: %1 => Org Path(Build Output Path)
:: %2 => Dest Path
:: %3 => file names
echo %1
echo %2
echo %3
@robocopy %1 %2 "%3" /NJH /NJS /NDL /FP /NP /NS /V /TEE /E
:: https://superuser.com/questions/280425/getting-robocopy-to-return-a-proper-exit-code
if %ERRORLEVEL% LSS 8 (
	set ERRORLEVEL=0
)

goto :eof

:Error
echo "Error :" %~1
set ERRORLEVEL=1

:eof
endlocal