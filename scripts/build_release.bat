@echo off
setlocal

set "ROOT=%~dp0.."
set "OUT_DIR=%ROOT%\dist\release"
set "OUT_EXE=%OUT_DIR%\Kalkulator.exe"

where cl >nul 2>nul
if errorlevel 1 (
    echo [ERROR] cl.exe tidak ditemukan. Jalankan dari "x64 Native Tools Command Prompt for VS".
    exit /b 1
)

where rc >nul 2>nul
if errorlevel 1 (
    echo [ERROR] rc.exe tidak ditemukan. Pastikan Windows SDK sudah terpasang.
    exit /b 1
)

if not exist "%OUT_DIR%" (
    mkdir "%OUT_DIR%"
)

pushd "%ROOT%"

echo [1/2] Compile resource...
rc /nologo app.rc
if errorlevel 1 (
    popd
    exit /b 1
)

echo [2/2] Build release binary...
cl /O2 /MT /EHsc /std:c++17 /DNDEBUG /nologo ^
 /Fe"%OUT_EXE%" ^
 "%ROOT%\main.cpp" ^
 "%ROOT%\fungsi\logika.cpp" ^
 "%ROOT%\fungsi\display\display.cpp" ^
 "%ROOT%\fungsi\history\history.cpp" ^
 "%ROOT%\fungsi\tombol\keyboard.cpp" ^
 "%ROOT%\fungsi\tombol\tombol.cpp" ^
 "%ROOT%\app.res" ^
 user32.lib gdi32.lib comctl32.lib ^
 /link /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF

if errorlevel 1 (
    popd
    exit /b 1
)

echo Release build selesai: "%OUT_EXE%"
popd
exit /b 0
