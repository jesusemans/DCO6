@echo off
echo ============================================================
echo  DCO6 VST3 - Build Script (Windows)
echo ============================================================
echo.

set VERSION=1.1
set PRODUCT_NAME=DCO6 v%VERSION%
set VST3_INSTALL_DIR=C:\Program Files\Common Files\VST3

set VSWHERE="%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"

if not exist %VSWHERE% (
    echo [ERROR] vswhere.exe no encontrado.
    echo Instala Visual Studio 2022 Build Tools desde:
    echo   https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022
    pause
    exit /b 1
)

for /f "usebackq tokens=*" %%i in (`%VSWHERE% -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
    set VS_PATH=%%i
)

if "%VS_PATH%"=="" (
    echo [ERROR] No se encontro Visual Studio con C++ Build Tools.
    echo Abre el Instalador de Visual Studio y agrega:
    echo   "Herramientas de compilacion de C++ para el escritorio"
    pause
    exit /b 1
)

echo [OK] Visual Studio encontrado: %VS_PATH%

set SDK_OK=0
for /f "tokens=*" %%i in ('dir /b /ad "%ProgramFiles(x86)%\Windows Kits\10\Lib" 2^>nul') do (
    set SDK_OK=1
)

if "%SDK_OK%"=="0" (
    echo.
    echo ============================================================
    echo  [ERROR] Windows SDK NO esta instalado.
    echo.
    echo  Sin el SDK no es posible compilar nada en Windows
    echo  ^(falta kernel32.lib, ucrt.lib, etc.^)
    echo.
    echo  SOLUCION - Elige una opcion:
    echo.
    echo  OPCION A - Desde el Instalador de Visual Studio:
    echo    1. Abre "Instalador de Visual Studio"
    echo    2. Clic en "Modificar" en Build Tools 2022
    echo    3. Marca: "Desarrollo de escritorio con C++"
    echo       ^(incluye el Windows 10/11 SDK automaticamente^)
    echo    4. Instala y vuelve a ejecutar este script
    echo.
    echo  OPCION B - Instalar el SDK directamente:
    echo    https://developer.microsoft.com/windows/downloads/windows-sdk/
    echo ============================================================
    pause
    exit /b 1
)

echo [OK] Windows SDK encontrado.
echo.

call "%VS_PATH%\VC\Auxiliary\Build\vcvarsall.bat" x64 >nul 2>&1
echo [OK] Entorno MSVC x64 activado.
echo.

set BUILD_DIR=build

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

echo [1/2] Configurando CMake...
cmake -S . -B "%BUILD_DIR%" -G "Visual Studio 17 2022" -A x64
if %ERRORLEVEL% neq 0 (
    echo.
    echo [ERROR] CMake configuration failed.
    echo Asegurate de tener CMake 3.22+ y Git instalados.
    pause
    exit /b 1
)

echo.
echo [2/2] Compilando Release (primera vez puede tardar 5-10 min descargando JUCE)...
cmake --build "%BUILD_DIR%" --config Release --parallel
if %ERRORLEVEL% neq 0 (
    echo.
    echo [ERROR] Compilacion fallida. Revisa los mensajes arriba.
    pause
    exit /b 1
)

echo.

echo [3/3] Instalando VST3...
set VST3_SRC=%BUILD_DIR%\DCO6_artefacts\Release\VST3\%PRODUCT_NAME%.vst3
set VST3_DST=%VST3_INSTALL_DIR%\%PRODUCT_NAME%.vst3

if exist "%VST3_SRC%" (
    if not exist "%VST3_INSTALL_DIR%" mkdir "%VST3_INSTALL_DIR%"
    xcopy /E /I /Y "%VST3_SRC%" "%VST3_DST%\" >nul 2>&1
    if %ERRORLEVEL% equ 0 (
        echo [OK] Instalado: %VST3_DST%
    ) else (
        echo [AVISO] No se pudo copiar automaticamente ^(requiere admin^).
        echo         Copia manualmente:
        echo           "%VST3_SRC%"
        echo         a:
        echo           "%VST3_INSTALL_DIR%\"
    )
) else (
    echo [AVISO] No se encontro el VST3 en: %VST3_SRC%
)

echo.
echo ============================================================
echo  Build EXITOSO! - %PRODUCT_NAME%
echo.
echo  VST3:       %VST3_SRC%
echo  Standalone: %BUILD_DIR%\DCO6_artefacts\Release\Standalone\%PRODUCT_NAME%.exe
echo  Instalado:  %VST3_DST%
echo.
echo  Versiones anteriores en "%VST3_INSTALL_DIR%" NO fueron modificadas.
echo ============================================================
pause
