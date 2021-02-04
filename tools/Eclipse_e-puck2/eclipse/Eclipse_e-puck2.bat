@echo off
set PATH=%cd%\tools\gnutools\bin;%PATH%

set /p gcc_path=<eclipse\gcc_path.txt
set PATH=%cd%%gcc_path%;%PATH%

start eclipse\eclipse.exe -data Workspace