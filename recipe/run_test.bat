@echo on
cl /nologo /I"%LIBRARY_INC%" test_font_match.c /Fe:test_font_match.exe /link /LIBPATH:"%LIBRARY_LIB%" fontconfig.lib
if errorlevel 1 exit /b 1
test_font_match.exe "%PREFIX%\fonts\DejaVuSans.ttf"
if errorlevel 1 exit /b 1
