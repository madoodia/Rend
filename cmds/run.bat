@REM # -------------------- #
@REM # (C)2024 madoodia.com #
@REM # -------------------- #

@echo off

SET PROJECT_NAME=%1
SET CURRENT_DIR=%~dp0

powershell -Command "& run %PROJECT_NAME%.exe" > "W:\output\render.ppm"
