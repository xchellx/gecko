@ECHO OFF
SET "dp0=%~dp0"
SET "dp0=%dp0:~0,-1%"
SET "ARGV=%*"
SET ISCMD=1
ECHO %CMDCMDLINE% | FINDSTR /I /C:"%~nx0" >NUL 2>&1 && SET ISCMD=0

CALL C:\msys64\msys2_shell.cmd -mingw64 -defterm -where "%dp0%" -no-start -shell bash -c ""$PWD/compile.sh" %ARGV%"

IF %ISCMD% EQU 0 PAUSE
