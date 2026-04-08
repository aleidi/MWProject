@echo off
REM 引擎路径
set ENGINE_PATH=D:/UE5.7
echo 引擎路径 : %ENGINE_PATH%
REM 项目路径
set PROJECT_PATH=%CD%\MW.uproject
echo 项目路径 : %PROJECT_PATH%

%ENGINE_PATH%/Engine/Build/BatchFiles/Build.bat  -projectfiles -project=%PROJECT_PATH% -game -engine -progress

pause