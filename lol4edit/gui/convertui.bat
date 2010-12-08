@echo off

FOR /R %~dp0ui\ %%G IN (*.ui) DO (
    echo Converting %%G...
    call %QTDIR%\bin\uic.exe -o "%~dp0\h\%%~nG.h" %%G

)


exit