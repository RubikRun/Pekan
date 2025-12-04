:: A batch script to generate a Visual Studio solution using CMake
:: You can pass "--clean" as an argument to remove the existing build directory (if any) before generation.

@echo off
setlocal enableextensions enabledelayedexpansion

:: Handle "--clean" flag
if "%~1"=="--clean" (
    if exist "build" (
        echo Removing existing build directory...
        rmdir /s /q "build"
    )
)

:: Find latest installed Visual Studio that has C++ tools
for /f "usebackq tokens=*" %%i in (`
    "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
`) do (
    set VS_DIR=%%i
)

if not defined VS_DIR (
    echo ERROR: Visual Studio is not installed or does not have C++ tools.
    exit /b 1
)

echo Found Visual Studio at: %VS_DIR%

:: Detect VS CMake path
set VS_CMAKE="%VS_DIR%\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"

if not exist %VS_CMAKE% (
    echo ERROR: CMake not found in Visual Studio, expected it here: %VS_CMAKE%
    exit /b 1
)

echo Using Visual Studio CMake at: %VS_CMAKE%

:: Setup MSVC environment
call "%VS_DIR%\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 (
    echo ERROR: Failed to run vcvars64.bat
    exit /b 1
)

:: Determine Visual Studio generator year
for /f "usebackq tokens=*" %%v in (`
    "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property catalog_productLineVersion
`) do (
    set VS_YEAR=%%v
)
if not defined VS_YEAR (
    set VS_YEAR=2022
    echo WARNING: Could not determine Visual Studio year, defaulting to %VS_YEAR%
)

set GENERATOR="Visual Studio %VS_YEAR%"

echo Using CMake generator: %GENERATOR%

:: Run VS CMake to generate a Visual Studio solution
%VS_CMAKE% -G %GENERATOR% -A x64 -S . -B build
if errorlevel 1 (
    echo ERROR: CMake generation failed.
    exit /b 1
)

echo Done!
