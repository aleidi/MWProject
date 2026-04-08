@echo off

if exist Intermediate (
    echo %CD%\Intermediate
    rmdir /s /q Intermediate
)

if exist Saved (
    echo %CD%\Saved
    rmdir /s /q Saved
)

for /d %%i in (Plugins\*) do (
    if exist "%%i\Intermediate" (
        echo %CD%\%%i\Intermediate
        rmdir /s /q "%%i\Intermediate" 2>nul
    )
)

if exist "%LOCALAPPDATA%\UnrealEngine\Common\DerivedDataCache" (
    echo %LOCALAPPDATA%\UnrealEngine\Common\DerivedDataCache
    rmdir /s /q "%LOCALAPPDATA%\UnrealEngine\Common\DerivedDataCache"
)

if exist "%LOCALAPPDATA%\UnrealEngine\Common\Zen\Data" (
    echo %LOCALAPPDATA%\UnrealEngine\Common\Zen\Data
    rmdir /s /q "%LOCALAPPDATA%\UnrealEngine\Common\Zen\Data"
)

pause