@echo off
rem VERSION 1.0

goto CHECK_PERMISSIONS

:CHECK_PERMISSIONS
    echo Administrative permissions required. Detecting permissions...

    net session >nul 2>&1
    if %errorLevel% == 0 (
        echo Success: Administrative permissions confirmed.
    ) else (
        echo Failure: Current permissions inadequate. Please right-click ETLLogger.bat and select the "Run as admin" option.
	echo Press any key to close this window.
	pause >nul
	GOTO DONE
    )

Set tracename=FocusriteTrace4
Set tracefile=FocusriteTrace.etl 

CHOICE /C 12 /M "Do you want to start logging? Press 1 to Start Logging or 2 to Cancel"

IF ERRORLEVEL 2 GOTO CANCEL
IF ERRORLEVEL 1 GOTO START

:START

rem Create the temp file with our GUIDs and flags.
(
echo {deadbeef-dead-beef-0000-feedfacebeef} 0xffffffff 255
echo {deadbeef-dead-beef-0001-feedfacebeef} 0xffffffff 255
echo {deadbeef-dead-beef-0002-feedfacebeef} 0xffffffff 255
echo {deadbeef-dead-beef-0003-feedfacebeef} 0xffffffff 255
echo {deadbeef-dead-beef-0004-feedfacebeef} 0xffffffff 255
echo {deadbeef-dead-beef-0005-feedfacebeef} 0xffffffff 255
)> "%~dp0_provs.tmp"

logman create trace -n %tracename% -m start stop -ow -o __tmptrace -nb 128 8192 -bs 64
logman update trace -n %tracename% -pf "%~dp0_provs.tmp"
logman start -n %tracename%
echo -----
echo Logging started.
echo Press any key to stop logging.
pause >nul
GOTO STOP

:STOP
logman stop -n %tracename% 
logman delete -n %tracename%
del "%~dp0_provs.tmp"
move /Y __tmptrace_000001.etl "%~dp0"
echo -----
echo Log saved.
echo Press any key to exit.
pause >nul
GOTO DONE

:CANCEL
echo Cancelling...
echo Press any key to close this window.
pause >nul
GOTO DONE

:DONE