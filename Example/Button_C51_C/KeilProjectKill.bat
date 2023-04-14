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

exit