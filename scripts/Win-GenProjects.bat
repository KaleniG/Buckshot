@echo off
pushd ..\
call vendor\Premake\premake5.exe vs2022
popd
IF %ERRORLEVEL% NEQ 0 (
  PAUSE
)