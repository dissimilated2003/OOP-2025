@echo off

set PROGRAM="..\x64\Debug\findtext.exe"

rem 1 -- test for desired string which exist in file once
echo Test1: 
%PROGRAM% "input.txt" "MLITA" | findstr /C:"1"
IF %ERRORLEVEL% NEQ 0 (
	goto err
)
echo Test 1 passed

rem 2 -- test for not exist desired string (lowercase)
echo Test2: 
%PROGRAM% "input.txt" "mlita" | findstr /C:"Text not found"
IF %ERRORLEVEL% NEQ 0 (
	goto err
)
echo Test 2 passed

rem 3 -- test for empty file
echo Test3: 
%PROGRAM% "empty.txt" "d" | findstr /C:"Text not found"
IF %ERRORLEVEL% NEQ 0 (
	goto err
)
echo Test 3 passed

rem 4 -- test for not exist file
echo Test4: 
%PROGRAM% "noExistFile.txt" "MLITA"
IF %ERRORLEVEL% NEQ 0 (
	echo Test 4 passed
)

rem 5 -- test for another desired string
echo Test5: 
%PROGRAM% "input.txt" "logika" | findstr /C:"3"
IF %ERRORLEVEL% NEQ 0 (
	goto err
)
echo Test 5 passed

rem 6 -- test for several desired strings, inline or between lines
echo Test6: 
%PROGRAM% "input.txt" "i" | findstr /C:"3" /C:"4" /C:"5"
IF %ERRORLEVEL% NEQ 0 (
	goto err
)
echo Test 6 passed

rem 7 -- analogue test for spaces
echo Test7: 
%PROGRAM% "input.txt" " " | findstr /C:"2" /C:"4"
IF %ERRORLEVEL% NEQ 0 (
	goto err
)
echo Test 7 passed

rem 8 -- test with not valid parametres number
echo Test8: 
%PROGRAM% "input.txt"
IF %ERRORLEVEL% NEQ 0 (
	echo Test 8 passed
)

rem 9 -- test with not valid parametres number
echo Test9: 
%PROGRAM% "input.txt" ""
IF %ERRORLEVEL% NEQ 0 (
	echo Test 9 passed
)


echo Program testing succeeded
exit /B 0

:err
echo Program testing failed
exit /B 1