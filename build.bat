@REM # --------------------- #
@REM # (C) 2026 madoodia.com #
@REM # --------------------- #

@ECHO OFF

SET DEBUG_MODE=%1
SET ROOT=%CD%

@REM Setup VS environment
@REM To suppress the output of the vcvarsall.bat, we redirect the output to NUL ( >NUL 2>&1)
CALL "E:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 >NUL 2>&1

IF %DEBUG_MODE%==1 (
    IF EXIST "%ROOT%\bin\Rend.exe" (
        ECHO Running in Debug Mode...
        devenv  "%ROOT%\bin\Rend.exe"
    ) ELSE (
        ECHO Debug build not found. Please build the project first.
    )
    EXIT /B 0
)
@echo Building the project...

@REM Build timer FIRST, outside timing (only if it doesn't exist)
SET TIMER_CXXFLAGS=-O2 -nologo -std:c++17 -D_CRT_SECURE_NO_WARNINGS
@REM  Need to link with advapi32.lib for registry functions
SET TIMER_LINKERFLAGS=kernel32.lib advapi32.lib

SET CXXFLAGS=-O2 -MTd -nologo -fp:fast -fp:except- -Gm- -GR- -EHa- -Zo -Oi -WX -W4 -FC -Z7 -std:c++17 -D_CRT_SECURE_NO_WARNINGS
SET LIBS= user32.lib gdi32.lib kernel32.lib winmm.lib opengl32.lib advapi32.lib
SET LINKERFLAGS=-opt:ref %LIBS%

if not exist "%ROOT%\bin\timer.exe" (
    cl %TIMER_CXXFLAGS% /Fo%ROOT%\build\\ "%ROOT%\tools\timer.cpp" /link %TIMER_LINKERFLAGS% /OUT:"%ROOT%\bin\timer.exe"
    EXIT /B 0
)

@REM NOW start timing

IF NOT EXIST "%ROOT%\build" (
    MKDIR "%ROOT%\build"
)

SET SOURCES="%ROOT%\src\main.cpp"

call "%ROOT%\bin\timer.exe" s
if exist "%ROOT%\bin\timer.exe" (
    cl %CXXFLAGS% /Fo%ROOT%\build\\ %SOURCES% /link %LINKERFLAGS% /OUT:"%ROOT%\bin\Rend.exe"
)
for /f "tokens=*" %%A in ('"%ROOT%\bin\timer.exe" e') do set ELAPSED=%%A
@echo Build Elapsed Time: %ELAPSED%

@REM copy %ROOT%\build\*.dll %ROOT%\bin > NUL 2>&1
@REM copy %ROOT%\build\*.exe %ROOT%\bin > NUL 2>&1

CALL "%ROOT%\bin\Rend.exe"

popd
