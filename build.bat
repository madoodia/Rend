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
@REM if not exist "bin" (
@REM     MKDIR "bin"
@REM )


IF NOT EXIST "%ROOT%\build" (
    MKDIR "%ROOT%\build"
)

SET SOURCES="%ROOT%\src\main.cpp"

pushd "%ROOT%\build"

@REM Optimized: /Od
@REM non Optimized: /Ox

SET CXXFLAGS=-O2 -MTd -nologo -fp:fast -fp:except- -Gm- -GR- -EHa- -Zo -Oi -WX -W4 -FC -Z7 -std:c++17
@REM SET CXXFLAGS=%CXXFLAGS% 
SET LINKERFLAGS=-opt:ref %LIBS%

SET LIBS= user32.lib gdi32.lib kernel32.lib winmm.lib opengl32.lib

cl %CXXFLAGS% %SOURCES% /link %LINKERFLAGS% /OUT:"%ROOT%\bin\Rend.exe"

@REM copy %ROOT%\build\*.dll %ROOT%\bin > NUL 2>&1
@REM copy %ROOT%\build\*.exe %ROOT%\bin > NUL 2>&1

CALL "%ROOT%\bin\Rend.exe"

popd
