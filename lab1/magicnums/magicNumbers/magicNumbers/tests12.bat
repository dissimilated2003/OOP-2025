@echo off

set PROGRAM="..\x64\Debug\findtext.exe"

rem 1 -- test for NAN
echo Test1: 
%PROGRAM% "MLITA"
IF %ERRORLEVEL% NEQ 0 (
	echo Test 1 passed
)


rem 2 -- test for empty input
echo Test2: 
%PROGRAM% ""
IF %ERRORLEVEL% NEQ 0 (
	echo Test 2 passed
)

rem 3 -- test for float input
echo Test3: 
%PROGRAM% "1212.3" | findstr /C:"1"
IF %ERRORLEVEL% NEQ 0 (
	echo Test 3 passed
)

rem 4 -- test for another desired string
echo Test4: 
%PROGRAM% "89898" | findstr /C:"Non-magic"
IF %ERRORLEVEL% NEQ 0 (
	goto err
)
echo Test 4 passed

rem 5 -- test for another desired string
echo Test5: 
%PROGRAM% "7" | findstr /C:"Non-magic"
IF %ERRORLEVEL% NEQ 0 (
	goto err
)
echo Test 5 passed

rem 6 -- test with zeros 
echo Test6: 
%PROGRAM% "0000" | findstr /C:"Non-magic"
IF %ERRORLEVEL% NEQ 0 (
	goto err
)
echo Test 6 passed

rem 7 -- test for invalid params number
echo Test7: 
%PROGRAM% | findstr /C:"Error"
IF %ERRORLEVEL% NEQ 0 (
	echo Test 7 passed
)

rem 8 -- test find magic from 0 to N
echo Test8: 
%PROGRAM% "-find 100000" | findstr /C:"Magic numbers from 0 to 100000"
IF %ERRORLEVEL% NEQ 0 (
	goto err
)
echo Test 8 passed


echo Program testing succeeded
exit /B 0

:err
echo Program testing failed
exit /B 1