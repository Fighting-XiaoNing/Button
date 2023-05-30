@echo off

del /s *.scvd
del /s *.uvgui.*
del /s *.uvguix.*
del /s JLinkLog.txt
del /s JLinkSettings.ini

del /s *.lst
del /s *.m51
del /s *.map

del /s *.__i
del /s *.axf
del /s *.crf
del /s *.d
del /s *.dep
del /s *.htm
del /s *.iex
del /s *.lnp
del /s *.o
del /s *.obj
del /s *.orc
del /s *.SBR
del /s *.sct

for /f "delims=" %%i in ('dir "%~dp0" /A-D /b /s') do (
if "%%~xi"=="" (del /s "%%~fi")
)

for /f "delims=" %%a in ('dir /ad/b/s') do (if "%%~nxa" equ ".vscode" rd /s /q "%%a")

cls

for %%i in ("%cd%") do (set name=%%~ni)
set g7zDir=C:\Program Files\7-Zip
set curDate=%date:~0,4%%date:~5,2%%date:~8,2%
set curTime=%time:~0,2%%time:~3,2%%time:~6,2%


:g7z_Compression

"%g7zDir%\7z.exe" a "%name%_%curDate%_%curTime%".7z "%cd%"

if %errorlevel% == 0 (
    exit
) else if %errorlevel% == 1 (
    cls
    echo The process cannot access the file because it is being used by another process.
    echo Please close the file and try again.
    echo The current compressed package is invalid!
    echo Compression failed!!!
    pause
    exit
) else (
    goto WinRAR_Compression
)


:WinRAR_Compression

start WinRAR.exe a -ep1 -agYYYYMMDD_HHMMSS "%name%"_.rar "%cd%"

setlocal enabledelayedexpansion
if !errorlevel! == 0 (
    setlocal disabledelayedexpansion
    exit
) else (
    setlocal disabledelayedexpansion
    goto Find_Compression
)


:Find_Compression

cls
if exist "C:\Program Files\WinRAR\WinRAR.exe" (
    set WinRarDir=C:\Program Files\WinRAR
) else if exist "D:\Program Files\WinRAR\WinRAR.exe" (
    set WinRarDir=D:\Program Files\WinRAR
) else if exist "E:\Program Files\WinRAR\WinRAR.exe" (
    set WinRarDir=E:\Program Files\WinRAR
) else if exist "F:\Program Files\WinRAR\WinRAR.exe" (
    set WinRarDir=F:\Program Files\WinRAR
) else if exist "G:\Program Files\WinRAR\WinRAR.exe" (
    set WinRarDir=G:\Program Files\WinRAR
) else (
    cls
    echo Please confirm that WinRAR is installed!
    echo If your WinRAR is found in "C:\Program Files\WinRAR\WinRAR.exe"
    echo Please input C:\Program Files\WinRAR
    echo And press enter!
    set /p WinRarDir=Please enter WinRAR installation path:
    echo Please wait for compression to complete!
)

"%WinRarDir%\WinRAR.exe" a -ep1 -agYYYYMMDD_HHMMSS "%name%"_.rar "%cd%"

if %errorlevel% == 0 (
    exit
) else (
    cls
    echo Compression failed!!!
    pause
    exit
)
