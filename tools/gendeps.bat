@ECHO OFF
cd %~dp0

premake5.exe --file=../premake5.lua vs2019
