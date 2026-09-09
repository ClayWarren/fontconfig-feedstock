@echo on
cl /nologo /I"%LIBRARY_INC%" test_font_match.c /Fe:test_font_match.exe /link /LIBPATH:"%LIBRARY_LIB%" fontconfig.lib
if errorlevel 1 exit /b 1
if "%target_platform%" == "win-arm64" (
  dumpbin /headers "%LIBRARY_BIN%\fontconfig-1.dll" | findstr /I /C:"AA64 machine (ARM64)"
  if errorlevel 1 exit /b 1
  dumpbin /headers test_font_match.exe | findstr /I /C:"AA64 machine (ARM64)"
  if errorlevel 1 exit /b 1
)
test_font_match.exe "%PREFIX%\fonts\DejaVuSans.ttf"
if errorlevel 1 exit /b 1
