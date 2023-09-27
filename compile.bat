@ECHO OFF

REM dp0 = current directory without leading slash
SET "dp0=%~dp0"
SET "dp0=%dp0:~0,-1%"
REM MINGW = path to msys2 shell to start mingw from
SET MINGW=C:\msys64\msys2_shell.cmd
REM ISCMD = if running interactively or not (to know if to PAUSE or not)
SET ISCMD=1
ECHO %CMDCMDLINE% | FINDSTR /I /C:"%~nx0" >NUL 2>&1 && SET ISCMD=0

REM check if msys2 shell exists
IF NOT EXIST %MINGW% GOTO :ELSE_MINGW
REM run msys2 shell and call MINGW from it
CALL %MINGW% -mingw64 -defterm -where "%dp0%" -no-start -shell bash -c "SA=($SHELL_ARGS) && $PWD/compile.sh ${SA[@]:5}" %*
REM if running interactively, pause and wait for key input, for user to check output
IF %ISCMD% EQU 0 PAUSE
REM set exit code to success
EXIT /B 0
:ELSE_MINGW
REM notify user that msys2 shell is missing and point to where to get it from to install it
ECHO ERROR: Missing MINGW, please install it from https://www.msys2.org
REM if running interactively, pause and wait for key input, for user to check output
IF %ISCMD% EQU 0 PAUSE
REM set exit code to error
EXIT /B 1
