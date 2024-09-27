@REM # --------------------- #
@REM # (C) 2024 madoodia.com #
@REM # --------------------- #

@echo off

SET PROJECT_NAME=%1

powershell -Command "& run devenv %PROJECT_NAME%.exe"



