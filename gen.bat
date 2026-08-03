:: A batch script to generate a Visual Studio solution using CMake
::
:: Usage:
::   gen.bat [--clean] [-DOPTION=VALUE ...]
::
:: Flags:
::   --clean    --->    Removes existing build directory before generation
::
:: CMake Options:
::   You can pass any CMake option using -DOPTION=VALUE format.
::   All -D options will be forwarded directly to CMake.
::   See CMakeLists.txt files for available project-specific options.
::
:: Examples:
::   gen.bat --clean
::   gen.bat -DWITH_DEMO_PROJECTS=OFF
::   gen.bat --clean -DPEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING=ON
::   gen.bat -DWITH_DEMO_PROJECTS=OFF -DPEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING=ON

@echo off
setlocal enableextensions enabledelayedexpansion

:: Initialize variables
set CLEAN_BUILD=0
set CMAKE_OPTIONS=

:: Parse all arguments
:parse_args
if "%~1"=="" goto args_done
if "%~1"=="--clean" (
	set CLEAN_BUILD=1
	shift
	goto parse_args
)
:: Collect all -D options for CMake
set ARG=%~1
if "!ARG:~0,2!"=="-D" (
	:: Check if the argument contains '='
	echo !ARG! | findstr /C:"=" >nul
	if errorlevel 1 (
		:: No '=' found, consume next argument as the value
		set CMAKE_OPTIONS=!CMAKE_OPTIONS! %~1=%~2
		shift
		shift
	) else (
		:: '=' found, use as-is
		set CMAKE_OPTIONS=!CMAKE_OPTIONS! %~1
		shift
	)
	goto parse_args
)
echo WARNING: Unknown argument '%~1' ignored
shift
goto parse_args
:args_done

:: Handle clean build
if %CLEAN_BUILD%==1 (
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
if defined CMAKE_OPTIONS (
	echo Using CMake options:%CMAKE_OPTIONS%
)

:: Run VS CMake to generate a Visual Studio solution
echo.
echo --------------------------------------------------------------------------------
echo Running CMake command:
echo %VS_CMAKE% -G %GENERATOR% -A x64 -S . -B build%CMAKE_OPTIONS%
echo --------------------------------------------------------------------------------
echo.
%VS_CMAKE% -G %GENERATOR% -A x64 -S . -B build%CMAKE_OPTIONS%
if errorlevel 1 (
	echo ERROR: CMake generation failed.
	exit /b 1
)

:: Run Ninja CMake to generate compile_commands.json for clangd
echo.
echo --------------------------------------------------------------------------------
echo Running Ninja CMake for compile_commands.json (clangd):
echo %VS_CMAKE% -G Ninja -DCMAKE_BUILD_TYPE=Debug -S . -B build/build-ninja%CMAKE_OPTIONS%
echo --------------------------------------------------------------------------------
echo.
%VS_CMAKE% -G Ninja -DCMAKE_BUILD_TYPE=Debug -S . -B build/build-ninja%CMAKE_OPTIONS%
if errorlevel 1 (
	echo WARNING: Ninja configuration failed. compile_commands.json will not be available for clangd.
) else (
	echo Ninja configuration succeeded. compile_commands.json is in build/build-ninja/
)

echo Done!
