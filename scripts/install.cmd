@echo off
setlocal

echo Building pmake...
mkdir build 2>nul
mkdir bin 2>nul
gcc -Wall -Wextra -std=c99 -Iinclude src\*.c -o bin\pmake.exe

echo Installing to C:\Program Files\pmake...
mkdir "%ProgramFiles%\pmake" >nul 2>nul
copy /Y bin\pmake.exe "%ProgramFiles%\pmake\" >nul

echo Checking if PATH already contains installation directory...
powershell -Command ^
  "$pmdir = [Environment]::ExpandEnvironmentVariables('%%ProgramFiles%%\\pmake');" ^
  "$envPath = [Environment]::GetEnvironmentVariable('Path', 'User');" ^
  "if (-not ($envPath.Split(';') -contains $pmdir)) {" ^
    "Write-Host 'Updating user PATH...';" ^
    "[Environment]::SetEnvironmentVariable('Path', $envPath + ';' + $pmdir, 'User');" ^
  "} else {" ^
    "Write-Host 'PATH already includes installation directory.';" ^
  "}"

echo.
echo Done! Open a new terminal to use pmake.
endlocal