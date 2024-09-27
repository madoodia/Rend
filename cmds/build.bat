@REM # --------------------- #
@REM # (C) 2024 madoodia.com #
@REM # --------------------- #

@ECHO OFF

SET ROOT=%1
SET PROJECT_NAME=%2

@REM Setup VS environment
@REM To suppress the output of the vcvarsall.bat, we redirect the output to NUL ( >NUL 2>&1)
@REM CALL "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 >NUL 2>&1
CALL "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

pushd "W:"
if exist "build" (
    rmdir /s /q "build"
)

if not exist "bin" (
    MKDIR "bin"
)

MKDIR "build"
popd
pushd "W:\build"

@REM Gathering sources, includes, and libs
SET SOURCES=W:\src\*.cpp
SET INCLUDES=W:\includes
SET LIBS_DIR=W:\libs
@REM SET CXXFLAGS=/EHsc /std:c++latest /D_CRT_SECURE_NO_WARNINGS

SET EXECUTABLE=W:\bin\%PROJECT_NAME%.exe

@REM WIN32
@REM _WINDOWS
@REM _GLFW_WIN32
@REM UNICODE
@REM _UNICODE
@REM _CRT_SECURE_NO_WARNINGS
@REM CMAKE_INTDIR="Debug"

Set MODE=Debug
if "%MODE%" == "Debug" (
    SET CXXFLAGS=-FC -EHsc -Zi -MDd -Od
    SET LIBS=user32.lib gdi32.lib kernel32.lib
) else (
    SET CXXFLAGS=-FC -EHsc -O2 -MD
    SET LIBS= user32.lib gdi32.lib kernel32.lib
)

@REM compiling and linking
cl %CXXFLAGS% -Fe%EXECUTABLE% %SOURCES% -I%INCLUDES% -link -LIBPATH:%LIBS_DIR% %LIBS%

popd

@REM There are 4 versions of the CRT link libraries present in vc\lib:
    @REM libcmt.lib: static CRT link library for a release build (/MT)
    @REM libcmtd.lib: static CRT link library for a debug build (/MTd)
    @REM msvcrt.lib: import library for the release DLL version of the CRT (/MD)
    @REM msvcrtd.lib: import library for the debug DLL version of the CRT (/MDd)