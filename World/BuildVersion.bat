@echo off
for /F "tokens=*" %%i in ('git describe --tags HEAD') do echo %%i && set VERSION=%%i
for /F "tokens=*" %%i in ('git show -s --format"=""%%ct"') do echo %%i && set VERSION2=%%i
>ver echo %VERSION% %VERSION2%