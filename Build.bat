@echo off
setlocal enabledelayedexpansion

if not exist "Engine\Build\CMakeLists.txt" (
    echo Please run the script from the root directory of the Duckers.
    exit /B 1
)

if "%~1"=="" (
    echo To use the script provide either project name or sample name:
    echo Build.bat [--Sample] ^<Project/sample name^>
    exit /B 1
)

if /I "%~1"=="--Sample" (
    if "%~2"=="" (
        echo Please provide sample name after "--Sample".
        exit /B 1
    )
    set "NAME=%~2"
    set "MODE=SAMPLE"
) else (
    set "NAME=%~1"
    set "MODE=PROJECT"
)

set "BUILD_DIR=Engine\CMake\%NAME%"
if not exist "%BUILD_DIR%" (
    mkdir "%BUILD_DIR%"
)
pushd "%BUILD_DIR%"

if "%MODE%"=="SAMPLE" (
    cmake --log-level=debug ..\..\Build\CMakeLists.txt -DDUCKERS_TARGET_SAMPLE_NAME="%NAME%"
) else (
    cmake --log-level=verbose ..\..\Build\CMakeLists.txt -DDUCKERS_TARGET_PROJECT_NAME="%NAME%"
)

if exist "compile_commands.json" (
    move /Y "compile_commands.json"  "..\..\..\" >nul
)

popd

endlocal
