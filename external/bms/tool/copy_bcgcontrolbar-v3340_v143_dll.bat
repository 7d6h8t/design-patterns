@echo off
@echo Copy BGCControlBar DLLs.
setlocal enableextensions

:: %1 => Destination Path(Build Output Path)
:: %2 => Configuration (Debug/Release)
:: %3 => PlatformTarget (Win32/x64)

set Configuration=%2
set PlatformTarget=%3
set OrgPath=
set OrgFileName=
:: if_find Internal => XXXX_Internal_XXX == true
if not x%Configuration:Debug=%==x%Configuration% (
    set OrgFileName=BCGCBPRO3340ud143.dll
    if not x%PlatformTarget:x64=%==x%PlatformTarget% (
        set OrgPath=%~dp0..\external\bcgControlBar\bin\x64
    ) else (
        call :Error "Check PlatformTarget"
    )
) else if not x%Configuration:Release=%==x%Configuration% (
    set OrgFileName=BCGCBPRO3340u143.dll
    if not x%PlatformTarget:x64=%==x%PlatformTarget% (
        set OrgPath=%~dp0..\external\bcgControlBar\bin\x64
    ) else (
        call :Error "Check PlatformTarget"
    )
) else (
    call :Error "Check Configuration"
)

@call %~dp0copy_files.bat %OrgPath% %1 %OrgFileName%
goto :eof

:Error
echo "Error :" %~1
set ERRORLEVEL=1

:eof
endlocal